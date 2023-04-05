
CREATE DATABASE IF NOT EXISTS webappDB;

USE webappDB;

CREATE TABLE IF NOT EXISTS config( name VARCHAR(255) PRIMARY KEY, 
                                   robotIp INT UNSIGNED,
                                   robotPort INT UNSIGNED,
                                   interfaceIp INT UNSIGNED,
                                   interfacePort INT UNSIGNED,
                                   description TEXT,
                                   created TIMESTAMP DEFAULT CURRENT_TIMESTAMP ); 
