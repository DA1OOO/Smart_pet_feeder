# 创建数据库
CREATE DATABASE miniprogram_db;

USE miniprogram_db;

DROP TABLE `user`;
/* 微信用户表，登录与发起业务时用 */
CREATE TABLE `user`  (
  `open_id` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT 'open_id',
#  `skey` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT 'skey',
  `create_time` timestamp(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  `last_visit_time` timestamp(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '最后登录时间',
  `session_key` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT 'session_key',
  `city` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '市',
  `province` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '省',
  `country` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '国',
  `avatar_url` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '头像',
  `gender` tinyint(11) NULL DEFAULT NULL COMMENT '性别',
  `nick_name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '网名',
  PRIMARY KEY (`open_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci COMMENT = '微信用户信息' ROW_FORMAT = Dynamic;

show tables;

select * from `user`;

/* 存储用户收藏的商品 */
CREATE TABLE `stared_items` (
	`item_id` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '京东商品id',
    `create_time` timestamp(0) NULL DEFAULT CURRENT_TIMESTAMP COMMENT '收藏时间',
    `price`  varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT '商品收藏时价格',
    `open_id` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL COMMENT 'open_id',
    
    PRIMARY KEY (`item_id`, `open_id`),
    FOREIGN KEY (`open_id`) REFERENCES `user`(`open_id`)
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci COMMENT = '用户收藏商品' ROW_FORMAT = Dynamic;

# 插入测试
insert into `stared_items` (`item_id`, `price`, `open_id`)
values('27596323', '64.5', 'oY1mB4sNMCAdYvEJdJu59ByTnS5U');

# 查询测试
select * from `stared_items`;

# 根据用户open_id查询收藏商品
select `item_id` from `stared_items`
where `open_id` = 'oY1mB4sNMCAdYvEJdJu59ByTnS5U';

