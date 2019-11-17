package ICE_Server;

import ICE_Logic.Game;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

    //************************* Class Fields ****************************//
    private int mode;//Determines the server mode (local/online)
    private int port=9090;//Default game port
    private ServerSocket serverSocket;//Server socket from Java

    //************************* Class Constructor ***********************//

    //Constructor, needs an integer that determines server mode
    public Server(int serverMode){
        this.mode=serverMode;
    }

    //********************** Setters and Getters ************************//

    //Returns port value
    public int getPort() {
        return this.port;
    }

    //Set a new value for the listening port
    public void setPort(int port) {
        this.port = port;
    }

    //Returns server mode value
    public int getMode(){return this.mode;}

    //********************** Start Server Method ************************//

    //Creates a server socket, server can handle 50 queued connections
    //Server game modes:
    //mode =0 -> Local Game
    //mode =1 -> Online Game

    public void start(){

        if(this.mode==0){
            this.localMultiplayer();

        }else if(this.mode==1){
            this.onlineMultiplayer();

        }else{
            System.out.println("**Server mode not recognize**");
        }

    }


    //*************************  Server Modes ***************************//

    //Local server mode, listen for clients requests and create a new thread
    //instance for every client. Then close the client socket.

    private void localMultiplayer(){

        Game game=new Game();//Creates a new Game room

        try {

            this.serverSocket = new ServerSocket(port);
            System.out.print("ICE_Server is listening in port: " + port+"\n");


            while (true) {
                //Starts listening for incoming client requests
                Socket socket = serverSocket.accept();
                System.out.print("New client connected\n");
                new Thread(socket,game).start();
            }

        }catch(IOException e){
            e.printStackTrace();
        }
    }

    //Online server mode, implementation with mqtt, not ready.

    private void onlineMultiplayer(){

        this.localMultiplayer();
    }

    //********************* Terminate Server Method *********************//

    //Termite the running server, all clients will be disconnected

    public void terminate(){

        try{
        this.serverSocket.close();
        }catch (IOException e){
            e.printStackTrace();
        }

    }

    //************************** Main Method ****************************//

    public static  void  main(String[] args){
        Server server=new Server(0);
        server.start();
    }
}
