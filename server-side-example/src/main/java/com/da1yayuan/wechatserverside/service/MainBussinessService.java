package com.da1yayuan.wechatserverside.service;

import com.alibaba.fastjson.JSONObject;
import com.da1yayuan.wechatserverside.consts.Constants;
import com.da1yayuan.wechatserverside.entity.User;
import com.da1yayuan.wechatserverside.mapper.UserMapper;
import com.da1yayuan.wechatserverside.response.GlobalResult;
import com.da1yayuan.wechatserverside.util.PythonRunnerUtil;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

@Slf4j
@Service
public class MainBussinessService {

    @Autowired
    private UserMapper userMapper;

    public GlobalResult mainBussinessService(String openId, String sessionKey, String url){

        // 响应结果
        JSONObject httpResData = new JSONObject();

        String realURL = null;

        /**·
         * 验证openid和sessionKey
         */
        User user = userMapper.selectById(openId);		//openid是用户数据表的主键，即可以唯一确定用户的标识
//		if(user == null) {
//			return new GlobalResult(501, "用户openid错误！", null);
//		}
//		if(!skey.equals(user.getSessionKey())) {
//			//System.out.println("skey: " + skey);
//			//System.out.println("user.getSessionKey: " + user.getSessionKey());
//			return new GlobalResult(502, "用户登录态错误！", null);
//		}

        /**
         * URL校验
         */
        Map<String, String> map = new HashMap<>(urlCheck(url));
        if(map.isEmpty()){
            return new GlobalResult(Constants.FAIL_CODE, "URL不是有效的京东商品链接.", null);
        }
        for(String id : map.keySet()){
            httpResData.put("id", id);
            realURL = map.get(id);
        }
        /**
         * 多线程 (1)运行python爬虫和分析 (2)获取商品基本信息
         */
        String spiderSourcePath = System.getProperty("user.dir") + "/JD-analysis-wechat-serverside-master/python-spider/";
        CountDownLatch latch = new CountDownLatch(2);	//2个子线程
        Thread th1 = new Thread(new PythonRunnerUtil(spiderSourcePath, realURL, openId, Constants.COMMENT_SPIDER_PATH, latch));//jd_comment.py中启动了sentiment_analysis.py
        th1.start();
        Thread th2 = new Thread(new PythonRunnerUtil(spiderSourcePath, realURL, openId, Constants.INFO_SPIDER_PATH, latch));
        th2.start();

        try {
            latch.await(4, TimeUnit.MINUTES);	//主进程等待，不超过3分钟
        } catch (InterruptedException e) {
            log.warn(String.valueOf(e));
        }
        /**
         * 处理结果：商品名称和图片URL, average_sentiment.txt, jd_ciyun.jpg
         */

        //读取jd_info.txt
        try {
            BufferedReader br = new BufferedReader(new FileReader(spiderSourcePath + Constants.INFO_FILE_PATH));
            String line = null;
            //读取第一行 商品名称
            if((line = br.readLine()) != null) {
                httpResData.put("itemName", line);
            }
            //读取第二行 商家名称
            if((line = br.readLine()) != null) {
                httpResData.put("shopName", line);
            }
            //读取第三行 商品主图URL
            if((line = br.readLine()) != null) {
                httpResData.put("imgUrl", line);
            }
            //关闭流
            br.close();
        } catch(Exception e) {
            log.warn(String.valueOf(e));
        }

        //读取average_sentiment.txt，即情感指数和评论数
        JSONObject tempObj = new JSONObject();
        try {
            BufferedReader br = new BufferedReader(new FileReader(spiderSourcePath + Constants.AVERAGE_SENTIMENT_FILE_PATH));
            String line = null;
            StringBuffer sb = new StringBuffer();
            while((line = br.readLine()) != null) {
                sb.append(line);
            }
            br.close();
            tempObj = JSONObject.parseObject(sb.toString());
        } catch(Exception e) {
            log.warn(String.valueOf(e));
        }
        httpResData.putAll(tempObj);

        //从jd_comment.csv截取评论
        JSONObject comments = new JSONObject();
        try {
            InputStreamReader isr = new InputStreamReader(new FileInputStream(spiderSourcePath + "jd_comment.csv"), Constants.FILE_CODE_GBK);
            BufferedReader br = new BufferedReader(isr);
            String line = br.readLine();
            //截取前4条评论
            for(int i = 0; line != null && i < 4; i++) {
                comments.put(Integer.valueOf(i).toString(), line);
                line = br.readLine();
            }
            isr.close();
        } catch(Exception e) {
            log.warn(String.valueOf(e));
        }
        httpResData.put("comments", comments);

        return new GlobalResult(Constants.SUCCESS_CODE, "查询成功", httpResData);
    }

    /**
     * URL校验
     */
    public Map<String, String> urlCheck(String url){
        String realURL = null;
        String id = null;
        Map<String, String> map = new HashMap<>();
        Pattern pattern = Pattern.compile(Constants.JD_REGEX);
        Matcher matcher = pattern.matcher(url);
        boolean isFind = matcher.find();
        if(isFind) {
            id = matcher.group("ID");
            realURL = "https://item.jd.com/" + matcher.group("ID") + ".html";
            map.put(id, realURL);
            return map;
        }
        return new HashMap<>();
    }
}
