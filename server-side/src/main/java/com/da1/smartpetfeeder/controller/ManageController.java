package com.da1.smartpetfeeder.controller;

import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

/**
 * 向硬件端发信号控制添加水、食物
 */
@RequestMapping("/add")
@Slf4j
@Controller
public class ManageController {
    @GetMapping("/water")
    @ResponseBody
    public Boolean addWater() {
        return true;
    }

    @GetMapping("/food")
    @ResponseBody
    public int addFood() {

        return 0;
    }
}
