package com.da1yayuan.wechatserverside.config;

import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.ResourceHandlerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurerAdapter;

/**
 * 静态资源重定向
 */
@Configuration
public class ResourceConfigAdapter extends WebMvcConfigurerAdapter {
    @Override
    public void addResourceHandlers(ResourceHandlerRegistry registry) {
        registry.addResourceHandler("\\upload\\**").addResourceLocations("C:\\Users\\Administrator\\Desktop\\IdeaWorkSpace\\JD-comments-analysis-wxminiprogram\\JD-analysis-wechat-serverside-master\\src\\main\\resources\\static");
    }
}