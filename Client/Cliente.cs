using System.Net;
using System.Net.Sockets;
using System.Text;


namespace Client{
     
    class Client{
        IPHostEntry host;
        IPAddress ipAddr;
        IPEndPoint endPoint;
        Socket sockClient;

        byte[] buffer;
    

        public Client(string ipServer, int port){
            host = Dns.GetHostEntry(ipServer);
            ipAddr = host.AddressList[0];  
            endPoint = new IPEndPoint(ipAddr, port);
            sockClient = new Socket(ipAddr.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            buffer = new byte[1024];
            sockClient.Connect(endPoint);
            
        }

        public void run(){
            string line = " ";
            Console.WriteLine("--> CLIENT CONNECT WITH SERVER...");
            while (sockClient.Connected && !line.Equals("exit")){
                sockClient.Receive(buffer);
                if (Encoding.UTF8.GetString(buffer, 0, 4).Equals("exit")){
                    break;
                }

                Console.WriteLine("--> server say :" + Encoding.UTF8.GetString(buffer, 0, buffer.Length));
                Console.Write("--> you say :");
                line = Console.ReadLine();
                sockClient.Send(Encoding.UTF8.GetBytes(line));
                buffer.AsSpan().Clear();
            }
            
        }

        static void Main(){
            Console.WriteLine("Digite o ip e a porta");
            Client client = new Client(Console.ReadLine(), int.Parse(Console.ReadLine()));
            client.run();
        }
    }
}

