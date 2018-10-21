package com.example.masag.piblinds;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.animation.ObjectAnimator;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Looper;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.jcraft.jsch.Channel;
import com.jcraft.jsch.ChannelExec;
import com.jcraft.jsch.ChannelSftp;
import com.jcraft.jsch.JSch;
import com.jcraft.jsch.Session;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;


public class MainActivity extends AppCompatActivity {

    // For handling new threads
    private static Handler nHandler = new Handler(Looper.getMainLooper());

    String user = "pi";
    String host = "192.168.1.32";
    String password = "Mauricio1";

    // For AP pi
//    String host = "192.168.0.1";
//    String password = "colombiano";

    String programPath;

    static boolean canPressBtn1 = true;
    static boolean canPressBtn2 = true;
    static boolean canPressBtn3 = true;
    static boolean tbtn1s = false;
    static boolean tbtn2s = false;
    static boolean tbtn3s = false;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btn1 = findViewById(R.id.button);
        Button btn2 = findViewById(R.id.button2);
        Button btn3 = findViewById(R.id.button3);

        // Handles the action when button 1 is pressed
        btn1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                programPath = "/home/pi/Desktop/blindControl 0";


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
        });

        // Button 2
        btn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                programPath = "/home/pi/Desktop/blindControl 1";

                    Toast.makeText(MainActivity.this, "Blind 2 Activated", Toast.LENGTH_SHORT).show();
                    System.out.println("BUTTON 2 PRESSED");

                    new AsyncTask<Integer, Void, Void>() {
                        @Override
                        protected Void doInBackground(Integer... params) {
                            System.out.println("BUTTON 2 PRESSED");

                            connectToPi();
                            return null;
                        }
                    }.execute(1);

            }
        });

        // Button 3
        btn3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                programPath = "/home/pi/Desktop/blindControl 2";

                    Toast.makeText(MainActivity.this, "Blind 3 Activated", Toast.LENGTH_SHORT).show();
                    System.out.println("BUTTON 3 PRESSED");

                    new AsyncTask<Integer, Void, Void>() {
                        @Override
                        protected Void doInBackground(Integer... params) {
                            System.out.println("BUTTON 2 PRESSED");

                            connectToPi();
                            return null;
                        }
                    }.execute(1);
            }
        });

        // Button 4
//        btn4.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
////                programPath = "/home/pi/Desktop/blindControl 2";
////
////                //if (canPressBtn3) {
////                    //canPressBtn3 = false;
////                    //buttonPushTimer3();
////                    Toast.makeText(MainActivity.this, "PI RESETTING", Toast.LENGTH_SHORT).show();
////                    //System.out.println("PI RESETTING");
////
////                    new AsyncTask<Integer, Void, Void>() {
////                        @Override
////                        protected Void doInBackground(Integer... params) {
////                            System.out.println("RESET PRESSED");
////
////                            connectToPi();
////                            return null;
////                        }
////                    }.execute(1);
////                new AsyncTask<Integer, Void, Void>() {
////                    @Override
////                    protected Void doInBackground(Integer... params) {
////                        System.out.println("SFTP CONNECTION INITIATED****************************************");
////
////                        getBlindStatus();
////                        return null;
////                    }
////                }.execute(1);
//
//            }
//        });


    }


    public void getBlindStatus() {
        try {
            JSch jsch = new JSch();
            Session session = jsch.getSession(user, host);
            session.setPassword(password);
            session.setConfig("StrictHostKeyChecking", "no");
            session.connect();

            ChannelSftp sftp = (ChannelSftp) session.openChannel("sftp");

            sftp.connect();

            InputStream stream = sftp.get("/home/pi/Desktop/blindStatus.txt");

            try {
                 InputStreamReader isr = new InputStreamReader(stream);
                System.out.println("HERE******************************************************************************");
                 BufferedReader br = new BufferedReader(isr);
                System.out.println("HERE******************************************************************************");
                // read from br

                if(br.readLine().equalsIgnoreCase("open"))
                    tbtn1s = true;
                else
                    tbtn1s = false;

                if(br.readLine().equalsIgnoreCase("open"))
                    tbtn2s = true;
                else
                    tbtn2s = false;

                if(br.readLine().equalsIgnoreCase("open"))
                    tbtn3s = true;
                else
                    tbtn3s = false;

//                System.out.println(br.readLine());
//                System.out.println(br.readLine());
//                System.out.println(br.readLine());

                stream.close();
            }
            catch(Exception e){
                e.printStackTrace();
            }
            finally{

                sftp.disconnect();
                session.disconnect();
            }

        } catch (Exception e) {
            e.printStackTrace();
        }

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
//            ((ChannelExec) channel).setCommand("/home/pi/Desktop/openBlinds");

            // For AP pi
            ((ChannelExec) channel).setCommand(programPath);
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
