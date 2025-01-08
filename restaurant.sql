/*
 Navicat Premium Dump SQL

 Source Server         : MySQL
 Source Server Type    : MySQL
 Source Server Version : 80040 (8.0.40)
 Source Host           : localhost:3306
 Source Schema         : restaurant

 Target Server Type    : MySQL
 Target Server Version : 80040 (8.0.40)
 File Encoding         : 65001

 Date: 08/01/2025 18:41:26
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for dishes
-- ----------------------------
DROP TABLE IF EXISTS `dishes`;
CREATE TABLE `dishes`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `category` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `price` double NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 55 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of dishes
-- ----------------------------
INSERT INTO `dishes` VALUES (1, '主食', '炒饭', 30);
INSERT INTO `dishes` VALUES (2, '主食', '面条', 25);
INSERT INTO `dishes` VALUES (3, '凉菜', '拍黄瓜', 15);
INSERT INTO `dishes` VALUES (4, '凉菜', '凉拌木耳', 20);
INSERT INTO `dishes` VALUES (5, '热菜', '宫保鸡丁', 40);
INSERT INTO `dishes` VALUES (6, '热菜', '红烧肉', 50);
INSERT INTO `dishes` VALUES (7, '汤类', '西红柿蛋汤', 18);
INSERT INTO `dishes` VALUES (8, '汤类', '紫菜蛋花汤', 15);
INSERT INTO `dishes` VALUES (9, '甜品', '红豆沙', 12);
INSERT INTO `dishes` VALUES (10, '甜品', '芒果布丁', 20);
INSERT INTO `dishes` VALUES (11, '饮品', '可乐', 8);
INSERT INTO `dishes` VALUES (12, '饮品', '橙汁', 10);
INSERT INTO `dishes` VALUES (13, '主食', '扬州炒饭', 35);
INSERT INTO `dishes` VALUES (14, '主食', '牛肉面', 40);
INSERT INTO `dishes` VALUES (15, '凉菜', '凉拌三丝', 18);
INSERT INTO `dishes` VALUES (16, '凉菜', '麻辣鸡丝', 22);
INSERT INTO `dishes` VALUES (17, '热菜', '鱼香肉丝', 45);
INSERT INTO `dishes` VALUES (18, '热菜', '糖醋里脊', 48);
INSERT INTO `dishes` VALUES (19, '汤类', '鸡汤', 25);
INSERT INTO `dishes` VALUES (20, '汤类', '排骨汤', 30);
INSERT INTO `dishes` VALUES (21, '甜品', '绿豆沙', 15);
INSERT INTO `dishes` VALUES (22, '甜品', '椰汁西米露', 22);
INSERT INTO `dishes` VALUES (23, '饮品', '雪碧', 8);
INSERT INTO `dishes` VALUES (24, '饮品', '苹果汁', 12);
INSERT INTO `dishes` VALUES (25, '主食', '鸡蛋炒饭', 32);
INSERT INTO `dishes` VALUES (26, '主食', '海鲜面', 45);
INSERT INTO `dishes` VALUES (27, '主食', '牛肉炒饭', 38);
INSERT INTO `dishes` VALUES (28, '主食', '虾仁炒饭', 42);
INSERT INTO `dishes` VALUES (29, '主食', '鸡肉面', 35);
INSERT INTO `dishes` VALUES (30, '凉菜', '凉拌黄瓜', 16);
INSERT INTO `dishes` VALUES (31, '凉菜', '凉拌豆腐丝', 18);
INSERT INTO `dishes` VALUES (32, '凉菜', '凉拌海带丝', 20);
INSERT INTO `dishes` VALUES (33, '凉菜', '凉拌茄子', 22);
INSERT INTO `dishes` VALUES (34, '凉菜', '凉拌萝卜丝', 15);
INSERT INTO `dishes` VALUES (35, '热菜', '红烧鱼', 55);
INSERT INTO `dishes` VALUES (36, '热菜', '清蒸鱼', 50);
INSERT INTO `dishes` VALUES (37, '热菜', '麻婆豆腐', 35);
INSERT INTO `dishes` VALUES (38, '热菜', '水煮肉片', 45);
INSERT INTO `dishes` VALUES (39, '热菜', '干锅牛肉', 60);
INSERT INTO `dishes` VALUES (40, '汤类', '鸡蛋汤', 12);
INSERT INTO `dishes` VALUES (41, '汤类', '鱼头汤', 28);
INSERT INTO `dishes` VALUES (42, '汤类', '排骨冬瓜汤', 30);
INSERT INTO `dishes` VALUES (43, '汤类', '牛肉汤', 35);
INSERT INTO `dishes` VALUES (44, '汤类', '羊肉汤', 40);
INSERT INTO `dishes` VALUES (45, '甜品', '水果沙拉', 25);
INSERT INTO `dishes` VALUES (46, '甜品', '冰淇淋', 18);
INSERT INTO `dishes` VALUES (47, '甜品', '蛋糕', 22);
INSERT INTO `dishes` VALUES (48, '甜品', '布丁', 20);
INSERT INTO `dishes` VALUES (49, '甜品', '奶酪', 28);
INSERT INTO `dishes` VALUES (50, '饮品', '绿茶', 10);
INSERT INTO `dishes` VALUES (51, '饮品', '红茶', 12);
INSERT INTO `dishes` VALUES (52, '饮品', '咖啡', 15);
INSERT INTO `dishes` VALUES (53, '饮品', '牛奶', 8);
INSERT INTO `dishes` VALUES (54, '饮品', '果汁', 10);

-- ----------------------------
-- Table structure for orders
-- ----------------------------
DROP TABLE IF EXISTS `orders`;
CREATE TABLE `orders`  (
  `order_id` int NOT NULL AUTO_INCREMENT,
  `table_id` int NULL DEFAULT NULL,
  `dish_id` int NULL DEFAULT NULL,
  `dish_name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `price` decimal(10, 2) NULL DEFAULT NULL,
  `quantity` int NULL DEFAULT NULL,
  `total_price` decimal(10, 2) NULL DEFAULT NULL,
  `status` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `creation_time` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`order_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of orders
-- ----------------------------

-- ----------------------------
-- Table structure for tables
-- ----------------------------
DROP TABLE IF EXISTS `tables`;
CREATE TABLE `tables`  (
  `table_id` int NOT NULL AUTO_INCREMENT,
  `seats` int NOT NULL,
  `status` tinyint(1) NOT NULL,
  PRIMARY KEY (`table_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 21 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of tables
-- ----------------------------
INSERT INTO `tables` VALUES (1, 4, 0);
INSERT INTO `tables` VALUES (2, 4, 0);
INSERT INTO `tables` VALUES (3, 4, 0);
INSERT INTO `tables` VALUES (4, 4, 0);
INSERT INTO `tables` VALUES (5, 4, 0);
INSERT INTO `tables` VALUES (6, 4, 0);
INSERT INTO `tables` VALUES (7, 6, 0);
INSERT INTO `tables` VALUES (8, 6, 0);
INSERT INTO `tables` VALUES (9, 8, 0);
INSERT INTO `tables` VALUES (10, 8, 0);

-- ----------------------------
-- Table structure for users
-- ----------------------------
DROP TABLE IF EXISTS `users`;
CREATE TABLE `users`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `username` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `password` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `role` int NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `username`(`username` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of users
-- ----------------------------
INSERT INTO `users` VALUES (1, 'admin', 'abc123', 2);
INSERT INTO `users` VALUES (2, 'ser', 'abc123', 1);

SET FOREIGN_KEY_CHECKS = 1;
