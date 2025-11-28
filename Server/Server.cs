using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Server{
    public class Server{
        private IPHostEntry host;
        private IPAddress address;
        private IPEndPoint endPoint;
        private Socket socketServer;

        private byte [] buffer;
        public Server(int PORT){
            host = Dns.GetHostEntry("localhost");
            address = host.AddressList[0];
            endPoint = new IPEndPoint(address,PORT);
            socketServer = new Socket(address.AddressFamily,SocketType.Stream,ProtocolType.Tcp);
            buffer = new byte[1024];
        }

        public void run(){
            socketServer.Bind(endPoint);
            socketServer.Listen();
            Socket connectionClient = socketServer.Accept();
            string message = "";
            Console.WriteLine("--> SERVER CONNECT WITH CLIENT...");
            while(!message.Equals("exit")){
                Console.Write("--> you say : ");    
                message = Console.ReadLine();
                connectionClient.Send(Encoding.UTF8.GetBytes(message));

                if (message.Equals("exit")){
                    break;
                }

                connectionClient.Receive(buffer);
                Console.WriteLine("--> client say : " + Encoding.UTF8.GetString(buffer, 0, buffer.Length));
                
                if (Encoding.UTF8.GetString(buffer, 0, 4).Equals("exit")){
                    break;
                }
                buffer.AsSpan().Clear();
            }
        }

        static void Main(string [] args){
            Console.WriteLine("Qual a porta : ");
            int port = int.Parse(Console.ReadLine());
            Server server = new Server(port);
            server.run();
        }
    }

}

