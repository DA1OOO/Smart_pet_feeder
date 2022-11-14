package com.da1yayuan.wechatserverside.controller;

import com.da1yayuan.wechatserverside.consts.Constants;
import com.da1yayuan.wechatserverside.response.GlobalResult;
import com.da1yayuan.wechatserverside.service.MainBussinessService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

@RequestMapping("/wechat")
@Slf4j
@Controller
/** 
 * @Description: 负责应用主要业务的控制器：京东商品页的爬虫与分析
 */
public class MainServiceController {
	@Autowired
	private MainBussinessService mainBussinessService;

	@PostMapping("/analyze")
	@ResponseBody
	public GlobalResult analyzeItem(@RequestParam(value = "openid", required = true) String openId,
									@RequestParam(value = "sessionKey", required = true)String sessionKey,
									@RequestParam(value = "url", required = true)String url)
	{
		return mainBussinessService.mainBussinessService(openId, sessionKey, url);
	}
}
