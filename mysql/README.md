
There is not much code for the mysql portion.  The big issue I had here was trying to figure out how to make Docker initialize my database using an SQL file.  My SQL file (dbSchema.sql) creates the database and the database table.  The trick to getting Docker to execute this script is in /docker-entrypoint-initdb.d/ in the mysql docker container.  If you can get the SQL script in that directory, Docker will execute it with an exception which I ran into.  If you also mount a persistant volume as I have and you bring up the container, then you add the SQL script into /docker-entrypoint-initdb.d/, then the script will not get executed.  This is because Docker will see that the volume already exists and stop everything else.  It will never do the initialization stuff.  You have to remove the persistant volume when changing the init stuff, so that the init stuff is called.  Once it is initialized, you don't want to execute it again anyway, so it's OK if it is a one-time thing.  You will see that all I have to do is set the volumes: lines as I have in docker-compose.yml and it just works.  The first line: 

"./mysql/dbSchema.sql:/docker-entrypoint-initdb.d/dbSchema.sql"

Puts dbSchema.sql into /docker-entrypoint-initdb.d in the container image.

The second line:

dbvolume:/virtual/data/mysql

Mounts a volume onto my computer in directory /virtual/data/mysql.  This is where all of the database data will reside.  It will persist outside of the container.  So, if I shutdown the container and restart it, the data will persist.  I do not have to reload the data everytime.