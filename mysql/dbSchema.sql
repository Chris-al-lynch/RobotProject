
CREATE DATABASE IF NOT EXISTS webappDB CHARACTER SET utf8;

USE webappDB;

CREATE TABLE IF NOT EXISTS config( name VARCHAR(255) PRIMARY KEY, 
                                   robotIp INT UNSIGNED,
                                   robotPort INT UNSIGNED,
                                   interfaceIp INT UNSIGNED,
                                   interfacePort INT UNSIGNED,
                                   description TEXT,
                                   created TIMESTAMP DEFAULT CURRENT_TIMESTAMP ); 
