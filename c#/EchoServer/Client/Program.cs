using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;

namespace EchoClient
{
    class Client
    {
        public static void Connect()
        {
            IPHostEntry host_entry = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress address = host_entry.AddressList.First(a => a.AddressFamily == AddressFamily.InterNetworkV6);
            IPEndPoint end_point = new IPEndPoint(address, 9090);

            Socket sender = new Socket(AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp);

            try
            {
                sender.Connect(end_point);
                Console.WriteLine("Socket connected to {0}", sender.RemoteEndPoint);

                byte[] data = Encoding.ASCII.GetBytes("Hello World!");
                Console.WriteLine("Sent : Hello World!");
                int byte_sent = sender.Send(data);

                string msg = null;
                while(true)
                {
                    byte[] buffer = new byte[1024];

                    int bytes_recived = sender.Receive(buffer);
                    if (bytes_recived > 0 )
                    {
                        msg += Encoding.ASCII.GetString(buffer, 0, bytes_recived);
                    }

                    if(msg.Contains(""))
                    {
                        break;
                    }
                }

                Console.WriteLine("Received : " + msg);
                sender.Shutdown(SocketShutdown.Both);
                sender.Close();
            }
            catch(Exception e)
            {
                Console.WriteLine(e);
                throw;
            }
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            Client.Connect();
        }
    }
}
