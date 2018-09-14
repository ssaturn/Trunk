using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;


namespace EchoServer
{
    enum net_error
    {
        success = 0,
        failed = 1,
    }
    class listener
    {
        void initialize()
        {
            _socket = new Socket(AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp);
        }
        public net_error proc(IPEndPoint end_point)
        {
            
            _socket.Bind(end_point);
            _socket.Listen(10);



            return net_error.success;
        }

        Socket _socket;
    }

    interface ISession
    {
        void Prerecv();
        void PrerecvCompleted(object sender, SocketAsyncEventArgs e);
        void Receive();
        void ReceiveCompleted(object sender, SocketAsyncEventArgs e);
    }



    public abstract class Session : ISession
    {
        public static readonly Int64 INVALID_ID = 0;
        public enum State
        {
            Disconnected = 0,
            Connected,
            Established
        }
        public Socket Socket
        {
            get { return socket; }
            set { socket = value; }
        }
        public Int64 ID
        {
            get { return id; }
            set { id = value; }
        }

        public static Int64 Issue()
        {
            return SeedID++;
        }

        public abstract void Prerecv();
        public abstract void PrerecvCompleted(object sender, SocketAsyncEventArgs e);
        public abstract void Receive();
        public abstract void ReceiveCompleted(object sender, SocketAsyncEventArgs e);

        public static Int64 SeedID = 1;
        private Int64 id = INVALID_ID;
        private State state = State.Disconnected;
        private Socket socket = null;
    }

    public class StateObject
    {
        // Client socket.  
        public Socket workSocket = null;
        // Size of receive buffer.  
        public const int BufferSize = 256;
        // Receive buffer.  
        public byte[] buffer = new byte[BufferSize];
        // Received data string.  
        public StringBuilder sb = new StringBuilder();
    }

    class SendBuffer
    {

    }

    class RecvBuffer
    {
        public byte[] _buffer;
        public int _head;
        public int _tail;
        int _size;

        public RecvBuffer(int size)
        {
            _buffer = new byte[size];
            _size = size;
        }

        //public byte[] Head
        //{
        //    get { return _buffer + _head; }
        //    set { _head = value; }
        //}


        public void SetBuffer(ref SocketAsyncEventArgs arg)
        {
            const int MTU = 512;
            var remain = _size - _tail;
            if ( remain < MTU)
            {
                _head = _tail = 0;
            }
            
            arg.SetBuffer(_buffer, _tail, remain);
        }

        public void Received(int bytesReceived)
        {
            _tail += bytesReceived;
        }


    }

    class UserSession : Session
    {
        private RecvBuffer _RecvBuffer = new RecvBuffer(8192);

        private static byte[] prerecv_buffer = new byte[0];
        public override void Receive()
        {
            SocketAsyncEventArgs arg = new SocketAsyncEventArgs();
            arg.Completed += new EventHandler<SocketAsyncEventArgs>(ReceiveCompleted);
            _RecvBuffer.SetBuffer(ref arg);
            Socket.ReceiveAsync(arg);
        }

        public override void ReceiveCompleted(object sender, SocketAsyncEventArgs e)
        {
            _RecvBuffer.Received(e.BytesTransferred);

            // 버퍼에 있는 값을 전부 가져오자.
            MemoryStream ms = new MemoryStream();
            ms.Write(_RecvBuffer._buffer, _RecvBuffer._head, _RecvBuffer._tail);
            byte[] message = ms.ToArray();
            Console.WriteLine("Recv Message : {0}", Encoding.UTF8.GetString(message));

            Prerecv();
        }

        public override  void Prerecv()
        {
            SocketAsyncEventArgs arg = new SocketAsyncEventArgs();
            arg.Completed += new EventHandler<SocketAsyncEventArgs>(PrerecvCompleted);
            arg.SetBuffer(new byte[0], 0, 0);
            Socket.ReceiveAsync(arg);
        }

        public override void PrerecvCompleted(object sender, SocketAsyncEventArgs e)
        {
            Receive();
        }


        public void PostSend()
        {

        }
    }
    
    class Server<T>
        where T : Session, new()
    {
        Socket _socket;

        //
        Dictionary<Int64, T> _sessions = new Dictionary<Int64, T>();
        private void AcceptComleted(object sender, SocketAsyncEventArgs e)
        {
            Socket client = e.AcceptSocket;

            T session = new T();
            session.ID = Session.Issue();
            session.Socket = client;
            _sessions.Add(session.ID, session);

            session.Prerecv();
        }

        public void Accept()
        {
            SocketAsyncEventArgs e = new SocketAsyncEventArgs();
            e.Completed += new EventHandler<SocketAsyncEventArgs>(AcceptComleted);

            Console.WriteLine("async accept!!");
            _socket.AcceptAsync(e);
        }

        public void Listen()
        {
            IPHostEntry host_entry = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress address = host_entry.AddressList.First(a => a.AddressFamily == AddressFamily.InterNetworkV6);
            IPEndPoint local_end_point = new IPEndPoint(address, 9090);

            _socket = new Socket(AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp);

            try
            {
                _socket.Bind(local_end_point);
                _socket.Listen(10);

                for(int i = 0; i < 10; ++i)
                {
                    Accept();
                }

                //    string msg = null;

                //    while(true)
                //    {
                //        //byte[] buffer = new byte[0];
                //        byte[] buffer = null;
                //        int byte_count = handler.Receive(buffer);

                //        byte[] buffer2 = new byte[1024];
                //        byte_count = handler.Receive(buffer2);


                //        if ( byte_count > 0 )
                //        {
                //            msg += Encoding.ASCII.GetString(buffer, 0, byte_count);
                //        }

                //        if ( msg.Contains(""))
                //        {
                //            break;
                //        }
                //    }

                //    Console.WriteLine("Received : {0}", msg);

                //    byte[] data = Encoding.ASCII.GetBytes(msg);

                //    Console.WriteLine("Send : {0}", msg);
                //    handler.Send(data);
                //    handler.Shutdown(SocketShutdown.Both);
                //    handler.Close();
                //}
            }
            catch(Exception e)
            {
                Console.WriteLine(e);
                throw;
            }
        }
    }
}
