package com.da1yayuan.wechatserverside.consts;

/**
 * 常量类
 * @Author:
 */
public interface Constants {

    /****************整型常量*******************/
    public static final int INTEGER_ONE = 1;

    /****************返回状态码*****************/
    public static final int SUCCESS_CODE = 200;

    public static final int FAIL_CODE = 555;

    /****************文件路径*******************/
    public static final String AVERAGE_SENTIMENT_FILE_PATH = "average_sentiment.txt";


    public static final String COMMENT_FILE_PATH = "jd_comment.csv";

    public static final String INFO_FILE_PATH = "jd_info.txt";

    public static final String COMMENT_SPIDER_PATH = "jd_comment.py";

    public static final String INFO_SPIDER_PATH = "jd_info.py";

    /*****************文件编码******************/
    public static final String FILE_CODE_GBK = "gbk";

    /*****************京东网址正则表达式****************/
    public static final String JD_REGEX = "https:(\\/)(\\/)item(\\.)(\\S*)jd(\\.)com(\\S*)(\\/)(?<ID>(\\d+))\\.html(.*)";

}
