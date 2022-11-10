package com.da1yayuan.wechatserverside.controller;

import com.da1yayuan.wechatserverside.response.GlobalResult;
import com.da1yayuan.wechatserverside.service.WechatLoginService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

@RequestMapping("/wechat")
@Controller
public class WechatLoginController {
    @Autowired
    private WechatLoginService wechatLoginService;

    @PostMapping("/weLogin")
    @ResponseBody
    public GlobalResult user_login(@RequestParam(value = "code", required = true) String code) {
        return wechatLoginService.wechatLogin(code);
    }
}
