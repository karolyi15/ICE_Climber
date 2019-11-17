package ICE_Server;

import ICE_Logic.Game;
import java.io.*;
import java.net.Socket;


public class Thread {

    //************************* Class Fields ****************************//
    private Socket socket;//Socket from Java
    private Game game;//Instance of Game class (Game room)
    private boolean running=true;//Determines when while loop stops

    //************************* Class Constructor ***********************//

    //Constructor, needs a socket to listen request and a Game instance
    //to update game data
    public Thread(Socket socket,Game game) {
        this.socket=socket;
        this.game=game;
    }

    //*********************** Start Client Method ***********************//

    //Handle Sockets connection (listening Requests)

    public void start(){
        try {
            //Read data from the client (read to byte array)
            InputStream input = socket.getInputStream();
            BufferedReader reader = new BufferedReader(new InputStreamReader(input),1024);

            //Write data as a byte array
            OutputStream output = socket.getOutputStream();
            //Converts byte array to text format
            PrintWriter writer = new PrintWriter(output, true);

            //Method Fields
            char inputData[]=new char[1024];//Buffer (Char array)

            //Do-While- Execute at least one time
            //Read client input and returns the updated game data
            do {

                reader.read(inputData,0,1024);
                System.out.println(String.valueOf(inputData).trim());
                this.game.update(String.valueOf(inputData).trim());

                writer.println(this.game.refresh());
                this.running=false;

            } while (running);

            //Close socket connection
            socket.close();
        }catch (IOException e) {
            e.printStackTrace();
        }
    }

}





