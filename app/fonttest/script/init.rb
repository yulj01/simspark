# we place all the application data in the data/ directory,
# so set this up here

blub = 0

if blub == 1
	importBundle 'bundle/filesystemzip';
	fileServer = get ('/sys/server/file');
	fileServer.mount ('FileSystemZIP', 'data.zip');
else
	importBundle 'bundle/filesystemstd';
	fileServer = get ('/sys/server/file');
	fileServer.mount ('FileSystemSTD', 'data/');
end

openglServer = new ('kerosin/OpenGLServer', '/sys/server/opengl');
openglServer.init();
