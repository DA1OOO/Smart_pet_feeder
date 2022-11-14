package com.da1.smartpetfeeder.controller;

import com.da1.smartpetfeeder.entity.DataEntity;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

/**
 * 接受硬件传递数据
 */
@RequestMapping("/data")
@Slf4j
@Controller
public class DataController {

    @Autowired
    DataEntity dataEntity;

    @GetMapping("/input")
    @ResponseBody
    public Boolean getData(@RequestParam(value = "weight", required = true) String weight) {
        dataEntity.setWeight(Integer.parseInt(weight));
        return Boolean.TRUE;
    }

    @GetMapping("/output")
    @ResponseBody
    public float returnData() {
        return dataEntity.getWeight();
    }
}
