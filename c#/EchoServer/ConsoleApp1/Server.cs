using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;


namespace EchoServer
{
    class Server
    {
        public static void Listen()
        {
            IPHostEntry host_entry = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress address = host_entry.AddressList.First(a => a.AddressFamily == AddressFamily.InterNetworkV6);
            IPEndPoint local_end_point = new IPEndPoint(address, 9090);

            Socket listener = new Socket(AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp);

            try
            {
                listener.Bind(local_end_point);
                listener.Listen(10);

                while(true)
                {
                    Console.WriteLine("Waiting for a connections ... ");
                    Socket handler = listener.Accept();
                    string msg = null;

                    while(true)
                    {
                        byte[] buffer = new byte[1024];
                        int byte_count = handler.Receive(buffer);

                        if( byte_count > 0 )
                        {
                            msg += Encoding.ASCII.GetString(buffer, 0, byte_count);
                        }

                        if ( msg.Contains(""))
                        {
                            break;
                        }
                    }

                    Console.WriteLine("Received : {0}", msg);

                    byte[] data = Encoding.ASCII.GetBytes(msg);

                    Console.WriteLine("Send : {0}", msg);
                    handler.Send(data);
                    handler.Shutdown(SocketShutdown.Both);
                    handler.Close();
                }
            }
            catch(Exception e)
            {
                Console.WriteLine(e);
                throw;
            }
        }
    }
}
