package com.da1yayuan.wechatserverside.response;

/**
 * @Description: 自定义响应数据结构
 * 				这个类是提供给门户，ios，安卓，微信商城用的
 * 				门户接受此类数据后需要使用本类的方法转换成对于的数据类型格式（类，或者list）
 * 				其他自行处理
 * 				200：表示成功
 * 				500：服务器无响应，错误信息在msg字段中
 * 				501：bean验证错误
 * 				502：用户登录态错误
 * 				555：其他错误：错误信息在msg字段中
 */
public class GlobalResult {

    // 响应业务状态
    private Integer status;

    // 响应消息
    private String msg;

    // 响应中的数据
    private Object respondData;
    
    private String ok;	// 不使用

    public static GlobalResult build(Integer status, String msg, Object respondData) {
        return new GlobalResult(status, msg, respondData);
    }

    public static GlobalResult ok(Object respondData) {
        return new GlobalResult(respondData);
    }

    public static GlobalResult ok() {
        return new GlobalResult(null);
    }
    
    public static GlobalResult errorMsg(String msg) {
        return new GlobalResult(500, msg, null);
    }
    
    public static GlobalResult errorMap(Object respondData) {
        return new GlobalResult(501, "error", respondData);
    }
    
    public static GlobalResult errorTokenMsg(String msg) {
        return new GlobalResult(502, msg, null);
    }
    
    public static GlobalResult errorException(String msg) {
        return new GlobalResult(555, msg, null);
    }

    public GlobalResult() {

    }

    public GlobalResult(Integer status, String msg, Object respondData) {
        this.status = status;
        this.msg = msg;
        this.respondData = respondData;
    }

    public GlobalResult(Object respondData) {
        this.status = 200;
        this.msg = "OK";
        this.respondData = respondData;
    }

    public Boolean isOK() {
        return this.status == 200;
    }

    public Integer getStatus() {
        return status;
    }

    public void setStatus(Integer status) {
        this.status = status;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public Object getData() {
        return respondData;
    }

    public void setData(Object respondData) {
        this.respondData = respondData;
    }

	public String getOk() {
		return ok;
	}

	public void setOk(String ok) {
		this.ok = ok;
	}

}
