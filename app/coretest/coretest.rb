importBundle "bundle/filesystemstd"

# mount a standard file system
fileServer = get ('/sys/server/file');
fileServer.mount ('FileSystemSTD', 'data/');

def testSoundSystem
	importBundle "bundle/soundsystemfmod"
	importBundle "bundle/soundsystembass"
	print "SoundServer Test\n"
	print "Creating SoundServer\n"
	soundServer = new ('kerosin/SoundServer', '/sys/server/sound');

	print "  Adding SoundSystem\n"
	#init "SoundSystemBASS"
	soundServer.init ('SoundSystemFMOD');
	#playEffect "shotgun.wav"
	#playModule "BTGATESF.XM"
	#playModule 'F:\Audio\Modules\Donna.xm'
	soundServer.playStream ('sinch.mp3');
end

testSoundSystem