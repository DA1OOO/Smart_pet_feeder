

# 查看用户
use mysql;
show tables;
select `Host`,`User` from user;

# 创建低权限用户
create user db_reader_writer identified by 'password';
grant select, update, delete, insert on miniprogram_db.* to db_reader_writer@'%';
flush privileges;

# 查看权限
show grants for db_reader_writer;