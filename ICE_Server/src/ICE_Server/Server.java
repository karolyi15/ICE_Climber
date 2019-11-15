package ICE_Server;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

    private int port=9090;
    private ServerSocket serverSocket;


    public void initServer(){

    try {

        //****************** Init ICE_Server.Server *********************//

        //Creates a server socket, handle 50 queued connections
        this.serverSocket = new ServerSocket(port);
        System.out.print("ICE_Server is listening in port: " + port+"\n");

        while (true) {
            //*************** Init ICE_Server.Client Socket ****************//

            //Starts listening for incoming client requests
            Socket socket = serverSocket.accept();
            System.out.print("New client connected\n");
            new Thread(socket).start();
        }

    }catch(IOException e){
        e.printStackTrace(); }

    }

    public void terminateServer(){

        //************** Terminate ICE_Server.Server *****************//

        //All clients will be disconnected
        try{
        this.serverSocket.close();
        }catch (IOException e){
            e.printStackTrace();
        }

    }

    public int getPort() {
        return port;
    }

    public void setPort(int port) {
        this.port = port;
    }

    public static  void  main(String[] args){
        Server server=new Server();
        server.initServer();
    }
}
