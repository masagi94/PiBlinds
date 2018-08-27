package com.example.masag.piblinds;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Looper;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.jcraft.jsch.Channel;
import com.jcraft.jsch.ChannelExec;
import com.jcraft.jsch.JSch;
import com.jcraft.jsch.Session;


public class MainActivity extends AppCompatActivity {

    // For handling new threads
    private static Handler nHandler = new Handler(Looper.getMainLooper());

    String user = "pi";
    String host = "xxx.xxx.x.xx";
    String password = "xxxxxxxxx";

    static boolean canPressBtn1 = true;
    static boolean canPressBtn2 = true;
    static boolean canPressBtn3 = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btn1 = findViewById(R.id.button1);
        Button btn2 = findViewById(R.id.button2);
        Button btn3 = findViewById(R.id.button3);

        // Handles the action when button 1 is pressed
        btn1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if(canPressBtn1){
                    canPressBtn1 = false;
                    buttonPushTimer1();
                    System.out.println("****************************ACTIVATING BLINDS****************************");
                    Toast.makeText(MainActivity.this, "Blind 1 Activated", Toast.LENGTH_SHORT).show();
                    new AsyncTask<Integer, Void, Void>(){
                        @Override
                        protected Void doInBackground(Integer... params){
                            System.out.println("BUTTON 1 PRESSED");

                            connectToPi();
                            return null;
                        }
                    }.execute(1);
                }
                else{
                    System.out.println("**************WAITING ON TIMER 1****************************");
                }
            }
        });

        // Button 2
        btn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(MainActivity.this, "Blind 2 Activated", Toast.LENGTH_SHORT).show();
                System.out.println("BUTTON 2 PRESSED");
            }
        });

        // Button 3
        btn3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(MainActivity.this, "Blind 3 Activated", Toast.LENGTH_SHORT).show();
                System.out.println("BUTTON 3 PRESSED");
            }
        });
    }


    /**
     * This method handles making the connection to the pi via SSH. Both the phone and the pi
     * must be connected to the same network (wifi) for this to work.
     * */
    public void connectToPi(){

        try{
            JSch jsch = new JSch();
            Session session = jsch.getSession(user,host);
            session.setPassword(password);
            session.setConfig("StrictHostKeyChecking", "no");
            session.connect();
            Channel channel = session.openChannel("exec");
            ((ChannelExec) channel).setCommand("/home/pi/Desktop/openBlinds");
            channel.connect();
            try{
                Thread.sleep(1000);
            }catch(Exception ee){
                System.out.println("ERROR: CONNECTION NOT ESTABLISHED***");
            }
            channel.disconnect();
            session.disconnect();
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }

    /**
     * These methods are threaded timers to prevent subsequent button presses while the pi
     * is already processing a button press.
     * */
    public static void buttonPushTimer1(){
        new Thread(new Runnable(){
            @Override
            public void run(){

                android.os.SystemClock.sleep(18000);
                nHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        canPressBtn1 = true;
                        System.out.println("**************TIMER 1 DONE");
                    }
                });
            }
        }).start();
    }

    public static void buttonPushTimer2(){
        new Thread(new Runnable(){
            @Override
            public void run(){

                android.os.SystemClock.sleep(18000);
                nHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        canPressBtn2 = true;
                        System.out.println("**************TIMER 2 DONE");
                    }
                });
            }
        }).start();
    }

    public static void buttonPushTimer3(){
        new Thread(new Runnable(){
            @Override
            public void run(){

                android.os.SystemClock.sleep(18000);
                nHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        canPressBtn3 = true;
                        System.out.println("**************TIMER 3 DONE");
                    }
                });
            }
        }).start();
    }


}
