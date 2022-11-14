package com.da1yayuan.wechatserverside.util;
import  java.io.BufferedReader;
import java.io.File;
import  java.io.IOException;  
import  java.io.InputStream;  
import  java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.concurrent.CountDownLatch;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Component;

@Slf4j
@Component
@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
public class PythonRunnerUtil implements Runnable {
	private static final String PYTHON_PATH_WIN = "C:\\Users\\Administrator\\AppData\\Local\\Programs\\Python\\Python310\\python.exe";
	private static final String PYTHON_PATH_LINUX = "/root/anaconda3/bin/python";
	private static final String PYTHON_PATH_MacOS = "/usr/local/bin/python3.7";
	final static SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");

	private String SPIDER_SOURCE_PATH;
	private String URL;
	private String openId;
	private String SOURCE_NAME = null;
	private CountDownLatch latch;
	
	public void run() {
		int exit_code = -1;		//python exit code
		log.info("=====> Runing Python Runtime...");
		String python_path = null;
		if(System.getProperty("os.name").equals("Windows 10")) {
			python_path = PYTHON_PATH_WIN;
		} else if(System.getProperty("os.name").equals("Linux")) {
			python_path = PYTHON_PATH_LINUX;
		} else if(System.getProperty("os.name").equals("Mac OS X")){
			python_path = PYTHON_PATH_MacOS;
		}
		String[] cmd = new String[] {python_path , SOURCE_NAME, URL,  openId};
        ProcessBuilder builder = new  ProcessBuilder();  
        builder.command(cmd);
        builder.directory(new File(SPIDER_SOURCE_PATH));
        
        try  {  
            Process pythonProcess = builder.start();
            final InputStream inputStream = pythonProcess.getInputStream();  
            final InputStream errorStream = pythonProcess.getErrorStream();
            new Thread() {  
                public void run() { 
                    BufferedReader br = new BufferedReader(new InputStreamReader(inputStream));  
                    try  {  
                        String lineA = null ;
                        while  ((lineA = br.readLine()) !=  null) {  
                            if  (lineA !=  null ) System.out.println(lineA);
                        }  
                    } catch (IOException e) {  
                        e.printStackTrace();  
                    }  
                }  
            }.start();
            new Thread() { 
                public void run() {
                    BufferedReader br2 = new BufferedReader(new InputStreamReader(errorStream));  
                    try {
                        String lineB = null ;  
                        while  ((lineB = br2.readLine()) !=  null) {
                            if  (lineB !=  null ) System.out.println(lineB);  
                        }  
                    } catch (IOException e) {  
                        e.printStackTrace();  
                    }  
                }
            }.start();
            exit_code = pythonProcess.waitFor();
            pythonProcess.destroy();
            inputStream.close();
            errorStream.close();
            latch.countDown();
        } catch  (Exception e) {  
            System.err.println(e);  
        }
        log.info("=====> Python program {} exit with code {}",SOURCE_NAME,exit_code);
    }
}
