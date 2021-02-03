IF(NOT DOCDIR)
    SET(DOCDIR "share/doc/lighttpd")
ENDIF(NOT DOCDIR)

SET(L_DOCS 
	accesslog.txt
	access.txt
	alias.txt
	authentication.txt
	cgi.txt
	CMakeLists.txt
	compress.txt
	configuration.txt
	dirlisting.txt
	evhost.txt
	expire.txt
	features.txt
	performance.txt
	plugins.txt
	redirect.txt
	rewrite.txt
	rrdtool.txt
	secdownload.txt
	security.txt
	setenv.txt
	simple-vhost.txt
	skeleton.txt
	ssi.txt
	ssl.txt
	state.txt
	status.txt
	traffic-shaping.txt
	trigger_b4_dl.txt
	userdir.txt
	webdav.txt
)

INSTALL(FILES ${L_DOCS}
	DESTINATION ${DOCDIR})
