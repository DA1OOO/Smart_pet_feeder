package com.da1yayuan.wechatserverside;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.builder.SpringApplicationBuilder;
import org.springframework.boot.web.servlet.support.SpringBootServletInitializer;

@MapperScan("com.da1yayuan.wechatserverside.mapper") //设置mapper接口的扫描包
@SpringBootApplication()
//SpringBootApplication(exclude= {DataSourceAutoConfiguration.class})
public class WechatServersideApplication extends SpringBootServletInitializer {

	public static void main(String[] args) {
		SpringApplication.run(WechatServersideApplication.class, args);
	}
	
	@Override	//打包springboot项目
	protected SpringApplicationBuilder configure(SpringApplicationBuilder builder) {
		return builder.sources(this.getClass());
	}

}