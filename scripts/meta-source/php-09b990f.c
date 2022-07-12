#define __COMPILE_0
#define __COMPILE_1
#define __COMPILE_2
#define __COMPILE_3
#define __COMPILE_4
#define __COMPILE_5
#define __COMPILE_6
#define __COMPILE_7
#define __COMPILE_8
#define __COMPILE_9
#define __COMPILE_10
#define __COMPILE_11
#define __COMPILE_12
#define __COMPILE_13
#define __COMPILE_14
#define __COMPILE_15
#define __COMPILE_17
#define __COMPILE_18
#define __COMPILE_19
#define __COMPILE_20
#define __COMPILE_21
#define __COMPILE_22
#define __COMPILE_24
#define __COMPILE_26
#define __COMPILE_30
#define __COMPILE_31
#define __COMPILE_32
#define __COMPILE_33
#define __COMPILE_34
#define __COMPILE_35
#define __COMPILE_36
#define __COMPILE_37
#define __COMPILE_38
#define __COMPILE_39
#define __COMPILE_40
#define __COMPILE_41
#define __COMPILE_42
#define __COMPILE_43
#define __COMPILE_44
#define __COMPILE_45
#define __COMPILE_46
#define __COMPILE_47
#define __COMPILE_48
#define __COMPILE_49
#define __COMPILE_50
#define __COMPILE_51
#define __COMPILE_52
#define __COMPILE_53
#define __COMPILE_54
#define __COMPILE_55
#define __COMPILE_56
#define __COMPILE_57
#define __COMPILE_58
#define __COMPILE_59
#define __COMPILE_60
#define __COMPILE_61
#define __COMPILE_62
#define __COMPILE_63
#define __COMPILE_64
#define __COMPILE_65
#define __COMPILE_66
#define __COMPILE_67
#define __COMPILE_68
#define __COMPILE_69
#define __COMPILE_70
// compile_fin
int __get_mutant(); int __is_neg(const char *location,char *lid,int count, ...); int __abst_hole(); char *__stat_write_init(const char *func_name);void __write_stat(char *str,const char *var_name,void *var_addr,int size); void __stat_file_close(const char *func_name,char *str);long long __mutate(const long long,const char *,const char *); int __trident_choice(char* lid, char* typestr,int* rvals, char** rvals_ids, int rvals_size,int** lvals, char** lvals_ids, int lvals_size);int __trident_output(char* id, char* typestr, int value);float fabs_trident(float a);int __choose(const char *);void *__var_select(unsigned int var_count,void *vars[]);void *__var_select_2(unsigned int var_count,void *vars[]);long long __const_select(unsigned int const_count, ...);void* memset(void*, int, unsigned long); 
/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2011 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Shane Caraveo <shane@php.net>                               |
   |          Wez Furlong <wez@thebrainroom.com>                          |
   +----------------------------------------------------------------------+
 */

/* $Id$ */

#define IS_EXT_MODULE

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "SAPI.h"

#define PHP_XML_INTERNAL
#include "zend_variables.h"
#include "ext/standard/php_string.h"
#include "ext/standard/info.h"
#include "ext/standard/file.h"

#if HAVE_LIBXML

#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/tree.h>
#include <libxml/uri.h>
#include <libxml/xmlerror.h>
#include <libxml/xmlsave.h>
#ifdef LIBXML_SCHEMAS_ENABLED
#include <libxml/relaxng.h>
#endif

#include "php_libxml.h"

#define PHP_LIBXML_ERROR 0
#define PHP_LIBXML_CTX_ERROR 1
#define PHP_LIBXML_CTX_WARNING 2

/* a true global for initialization */
static int _php_libxml_initialized = 0;
static int _php_libxml_per_request_initialization = 1;

typedef struct _php_libxml_func_handler {
	php_libxml_export_node export_func;
} php_libxml_func_handler;

static HashTable php_libxml_exports;

static ZEND_DECLARE_MODULE_GLOBALS(libxml)
static PHP_GINIT_FUNCTION(libxml);

static PHP_FUNCTION(libxml_set_streams_context);
static PHP_FUNCTION(libxml_use_internal_errors);
static PHP_FUNCTION(libxml_get_last_error);
static PHP_FUNCTION(libxml_clear_errors);
static PHP_FUNCTION(libxml_get_errors);
static PHP_FUNCTION(libxml_disable_entity_loader);

static zend_class_entry *libxmlerror_class_entry;

/* {{{ dynamically loadable module stuff */
#ifdef COMPILE_DL_LIBXML
ZEND_GET_MODULE(libxml)
#endif /* COMPILE_DL_LIBXML */
/* }}} */

/* {{{ function prototypes */
static PHP_MINIT_FUNCTION(libxml);
static PHP_RINIT_FUNCTION(libxml);
static PHP_MSHUTDOWN_FUNCTION(libxml);
static PHP_RSHUTDOWN_FUNCTION(libxml);
static PHP_MINFO_FUNCTION(libxml);

/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO(arginfo_libxml_set_streams_context, 0)
	ZEND_ARG_INFO(0, context)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_libxml_use_internal_errors, 0, 0, 0)
	ZEND_ARG_INFO(0, use_errors)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_libxml_get_last_error, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_libxml_get_errors, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_libxml_clear_errors, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_libxml_disable_entity_loader, 0, 0, 0)
	ZEND_ARG_INFO(0, disable)
ZEND_END_ARG_INFO()

/* }}} */

/* {{{ extension definition structures */
static const zend_function_entry libxml_functions[] = {
	PHP_FE(libxml_set_streams_context, arginfo_libxml_set_streams_context)
	PHP_FE(libxml_use_internal_errors, arginfo_libxml_use_internal_errors)
	PHP_FE(libxml_get_last_error, arginfo_libxml_get_last_error)
	PHP_FE(libxml_clear_errors, arginfo_libxml_clear_errors)
	PHP_FE(libxml_get_errors, arginfo_libxml_get_errors)
	PHP_FE(libxml_disable_entity_loader, arginfo_libxml_disable_entity_loader)
	{NULL, NULL, NULL}
};

zend_module_entry libxml_module_entry = {
	STANDARD_MODULE_HEADER,
	"libxml",                /* extension name */
	libxml_functions,        /* extension function list */
	PHP_MINIT(libxml),       /* extension-wide startup function */
	PHP_MSHUTDOWN(libxml),   /* extension-wide shutdown function */
	PHP_RINIT(libxml),       /* per-request startup function */
	PHP_RSHUTDOWN(libxml),   /* per-request shutdown function */
	PHP_MINFO(libxml),       /* information function */
	NO_VERSION_YET,
	PHP_MODULE_GLOBALS(libxml), /* globals descriptor */
	PHP_GINIT(libxml),          /* globals ctor */
	NULL,                       /* globals dtor */
	NULL,                       /* post deactivate */
	STANDARD_MODULE_PROPERTIES_EX
};

/* }}} */

/* {{{ internal functions for interoperability */
static int php_libxml_clear_object(php_libxml_node_object *object TSRMLS_DC)
{
	if (object->properties) {
		object->properties = NULL;
	}
	php_libxml_decrement_node_ptr(object TSRMLS_CC);
	return php_libxml_decrement_doc_ref(object TSRMLS_CC);
}

static int php_libxml_unregister_node(xmlNodePtr nodep TSRMLS_DC)
{
	php_libxml_node_object *wrapper;

	php_libxml_node_ptr *nodeptr = nodep->_private;

	if (nodeptr != NULL) {
		wrapper = nodeptr->_private;
		if (wrapper) {
			php_libxml_clear_object(wrapper TSRMLS_CC);
		} else {
			if (nodeptr->node != NULL && nodeptr->node->type != XML_DOCUMENT_NODE) {
				nodeptr->node->_private = NULL;
			}
			nodeptr->node = NULL;
		}
	}

	return -1;
}

static void php_libxml_node_free(xmlNodePtr node)
{
	if(node) {
		if (node->_private != NULL) {
			((php_libxml_node_ptr *) node->_private)->node = NULL;
		}
		switch (node->type) {
			case XML_ATTRIBUTE_NODE:
				xmlFreeProp((xmlAttrPtr) node);
				break;
			case XML_ENTITY_DECL:
			case XML_ELEMENT_DECL:
			case XML_ATTRIBUTE_DECL:
				break;
			case XML_NOTATION_NODE:
				/* These require special handling */
				if (node->name != NULL) {
					xmlFree((char *) node->name);
				}
				if (((xmlEntityPtr) node)->ExternalID != NULL) {
					xmlFree((char *) ((xmlEntityPtr) node)->ExternalID);
				}
				if (((xmlEntityPtr) node)->SystemID != NULL) {
					xmlFree((char *) ((xmlEntityPtr) node)->SystemID);
				}
				xmlFree(node);
				break;
			case XML_NAMESPACE_DECL:
				if (node->ns) {
					xmlFreeNs(node->ns);
					node->ns = NULL;
				}
				node->type = XML_ELEMENT_NODE;
			default:
				xmlFreeNode(node);
		}
	}
}

static void php_libxml_node_free_list(xmlNodePtr node TSRMLS_DC)
{
	xmlNodePtr curnode;

	if (node != NULL) {
		curnode = node;
		while (curnode != NULL) {
			node = curnode;
			switch (node->type) {
				/* Skip property freeing for the following types */
				case XML_NOTATION_NODE:
					break;
				case XML_ENTITY_REF_NODE:
					php_libxml_node_free_list((xmlNodePtr) node->properties TSRMLS_CC);
					break;
				case XML_ATTRIBUTE_NODE:
    					if ((node->doc != NULL) && (((xmlAttrPtr) node)->atype == XML_ATTRIBUTE_ID)) {
	    					xmlRemoveID(node->doc, (xmlAttrPtr) node);
    					}
				case XML_ATTRIBUTE_DECL:
				case XML_DTD_NODE:
				case XML_DOCUMENT_TYPE_NODE:
				case XML_ENTITY_DECL:
				case XML_NAMESPACE_DECL:
				case XML_TEXT_NODE:
					php_libxml_node_free_list(node->children TSRMLS_CC);
					break;
				default:
					php_libxml_node_free_list(node->children TSRMLS_CC);
					php_libxml_node_free_list((xmlNodePtr) node->properties TSRMLS_CC);
			}

			curnode = node->next;
			xmlUnlinkNode(node);
			if (php_libxml_unregister_node(node TSRMLS_CC) == 0) {
				node->doc = NULL;
			}
			php_libxml_node_free(node);
		}
	}
}

/* }}} */

/* {{{ startup, shutdown and info functions */
static PHP_GINIT_FUNCTION(libxml)
{
	libxml_globals->stream_context = NULL;
	libxml_globals->error_buffer.c = NULL;
	libxml_globals->error_list = NULL;
}

/* Channel libxml file io layer through the PHP streams subsystem.
 * This allows use of ftps:// and https:// urls */

static void *php_libxml_streams_IO_open_wrapper(const char *filename, const char *mode, const int read_only)
{
	php_stream_statbuf ssbuf;
	php_stream_context *context = NULL;
	php_stream_wrapper *wrapper = NULL;
	char *resolved_path, *path_to_open = NULL;
	void *ret_val = NULL;
	int isescaped=0;
	xmlURI *uri;

	TSRMLS_FETCH();

	uri = xmlParseURI((xmlChar *)filename);
	if (uri && (uri->scheme == NULL || (xmlStrncmp(uri->scheme, "file", 4) == 0))) {
		resolved_path = xmlURIUnescapeString(filename, 0, NULL);
		isescaped = 1;
	} else {
		resolved_path = (char *)filename;
	}

	if (uri) {
		xmlFreeURI(uri);
	}

	if (resolved_path == NULL) {
		return NULL;
	}

	/* logic copied from _php_stream_stat, but we only want to fail
	   if the wrapper supports stat, otherwise, figure it out from
	   the open.  This logic is only to support hiding warnings
	   that the streams layer puts out at times, but for libxml we
	   may try to open files that don't exist, but it is not a failure
	   in xml processing (eg. DTD files)  */
	wrapper = php_stream_locate_url_wrapper(resolved_path, &path_to_open, 0 TSRMLS_CC);
	if (wrapper && read_only && wrapper->wops->url_stat) {
		if (wrapper->wops->url_stat(wrapper, path_to_open, PHP_STREAM_URL_STAT_QUIET, &ssbuf, NULL TSRMLS_CC) == -1) {
			if (isescaped) {
				xmlFree(resolved_path);
			}
			return NULL;
		}
	}

	if (LIBXML(stream_context)) {
		context = zend_fetch_resource(&LIBXML(stream_context) TSRMLS_CC, -1, "Stream-Context", NULL, 1, php_le_stream_context(TSRMLS_C));
	}

	ret_val = php_stream_open_wrapper_ex(path_to_open, (char *)mode, REPORT_ERRORS, NULL, context);
	if (isescaped) {
		xmlFree(resolved_path);
	}
	return ret_val;
}

static void *php_libxml_streams_IO_open_read_wrapper(const char *filename)
{
	return php_libxml_streams_IO_open_wrapper(filename, "rb", 1);
}

static void *php_libxml_streams_IO_open_write_wrapper(const char *filename)
{
	return php_libxml_streams_IO_open_wrapper(filename, "wb", 0);
}

static int php_libxml_streams_IO_read(void *context, char *buffer, int len)
{
	TSRMLS_FETCH();
	return php_stream_read((php_stream*)context, buffer, len);
}

static int php_libxml_streams_IO_write(void *context, const char *buffer, int len)
{
	TSRMLS_FETCH();
	return php_stream_write((php_stream*)context, buffer, len);
}

static int php_libxml_streams_IO_close(void *context)
{
	TSRMLS_FETCH();
	return php_stream_close((php_stream*)context);
}

static xmlParserInputBufferPtr
php_libxml_input_buffer_noload(const char *URI, xmlCharEncoding enc)
{
	return NULL;
}

static xmlParserInputBufferPtr
php_libxml_input_buffer_create_filename(const char *URI, xmlCharEncoding enc)
{
	xmlParserInputBufferPtr ret;
	void *context = NULL;

	if (URI == NULL)
		return(NULL);

	context = php_libxml_streams_IO_open_read_wrapper(URI);

	if (context == NULL) {
		return(NULL);
	}

	/* Allocate the Input buffer front-end. */
	ret = xmlAllocParserInputBuffer(enc);
	if (ret != NULL) {
		ret->context = context;
		ret->readcallback = php_libxml_streams_IO_read;
		ret->closecallback = php_libxml_streams_IO_close;
	} else
		php_libxml_streams_IO_close(context);

	return(ret);
}

static xmlOutputBufferPtr
php_libxml_output_buffer_create_filename(const char *URI,
                              xmlCharEncodingHandlerPtr encoder,
                              int compression ATTRIBUTE_UNUSED)
{
	xmlOutputBufferPtr ret;
	xmlURIPtr puri;
	void *context = NULL;
	char *unescaped = NULL;

	if (URI == NULL)
		return(NULL);

	puri = xmlParseURI(URI);
	if (puri != NULL) {
		if (puri->scheme != NULL)
			unescaped = xmlURIUnescapeString(URI, 0, NULL);
		xmlFreeURI(puri);
	}

	if (unescaped != NULL) {
		context = php_libxml_streams_IO_open_write_wrapper(unescaped);
		xmlFree(unescaped);
	}

	/* try with a non-escaped URI this may be a strange filename */
	if (context == NULL) {
		context = php_libxml_streams_IO_open_write_wrapper(URI);
	}

	if (context == NULL) {
		return(NULL);
	}

	/* Allocate the Output buffer front-end. */
	ret = xmlAllocOutputBuffer(encoder);
	if (ret != NULL) {
		ret->context = context;
		ret->writecallback = php_libxml_streams_IO_write;
		ret->closecallback = php_libxml_streams_IO_close;
	}

	return(ret);
}

static int _php_libxml_free_error(xmlErrorPtr error)
{
	/* This will free the libxml alloc'd memory */
	xmlResetError(error);
	return 1;
}

static void _php_list_set_error_structure(xmlErrorPtr error, const char *msg)
{
	xmlError error_copy;
	int ret;

	TSRMLS_FETCH();

	memset(&error_copy, 0, sizeof(xmlError));

	if (error) {
		ret = xmlCopyError(error, &error_copy);
	} else {
		error_copy.domain = 0;
		error_copy.code = XML_ERR_INTERNAL_ERROR;
		error_copy.level = XML_ERR_ERROR;
		error_copy.line = 0;
		error_copy.node = NULL;
		error_copy.int1 = 0;
		error_copy.int2 = 0;
		error_copy.ctxt = NULL;
		error_copy.message = xmlStrdup(msg);
		error_copy.file = NULL;
		error_copy.str1 = NULL;
		error_copy.str2 = NULL;
		error_copy.str3 = NULL;
		ret = 0;
	}

	if (ret == 0) {
		zend_llist_add_element(LIBXML(error_list), &error_copy);
	}
}

static void php_libxml_ctx_error_level(int level, void *ctx, const char *msg TSRMLS_DC)
{
	xmlParserCtxtPtr parser;

	parser = (xmlParserCtxtPtr) ctx;

	if (parser != NULL && parser->input != NULL) {
		if (parser->input->filename) {
			php_error_docref(NULL TSRMLS_CC, level, "%s in %s, line: %d", msg, parser->input->filename, parser->input->line);
		} else {
			php_error_docref(NULL TSRMLS_CC, level, "%s in Entity, line: %d", msg, parser->input->line);
		}
	}
}

void php_libxml_issue_error(int level, const char *msg TSRMLS_DC)
{
	if (LIBXML(error_list)) {
		_php_list_set_error_structure(NULL, msg);
	} else {
		php_error_docref(NULL TSRMLS_CC, level, "%s", msg);
	}
}

static void php_libxml_internal_error_handler(int error_type, void *ctx, const char **msg, va_list ap)
{
	char *buf;
	int len, len_iter, output = 0;

	TSRMLS_FETCH();

	len = vspprintf(&buf, 0, *msg, ap);
	len_iter = len;

	/* remove any trailing \n */
	while (len_iter && buf[--len_iter] == '\n') {
		buf[len_iter] = '\0';
		output = 1;
	}

	smart_str_appendl(&LIBXML(error_buffer), buf, len);

	efree(buf);

	if (output == 1) {
		if (LIBXML(error_list)) {
			_php_list_set_error_structure(NULL, LIBXML(error_buffer).c);
		} else {
			switch (error_type) {
				case PHP_LIBXML_CTX_ERROR:
					php_libxml_ctx_error_level(E_WARNING, ctx, LIBXML(error_buffer).c TSRMLS_CC);
					break;
				case PHP_LIBXML_CTX_WARNING:
					php_libxml_ctx_error_level(E_NOTICE, ctx, LIBXML(error_buffer).c TSRMLS_CC);
					break;
				default:
					php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s", LIBXML(error_buffer).c);
			}
		}
		smart_str_free(&LIBXML(error_buffer));
	}
}

PHP_LIBXML_API void php_libxml_ctx_error(void *ctx, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	php_libxml_internal_error_handler(PHP_LIBXML_CTX_ERROR, ctx, &msg, args);
	va_end(args);
}

PHP_LIBXML_API void php_libxml_ctx_warning(void *ctx, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	php_libxml_internal_error_handler(PHP_LIBXML_CTX_WARNING, ctx, &msg, args);
	va_end(args);
}

PHP_LIBXML_API void php_libxml_structured_error_handler(void *userData, xmlErrorPtr error)
{
	_php_list_set_error_structure(error, NULL);

	return;
}

PHP_LIBXML_API void php_libxml_error_handler(void *ctx, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	php_libxml_internal_error_handler(PHP_LIBXML_ERROR, ctx, &msg, args);
	va_end(args);
}


PHP_LIBXML_API void php_libxml_initialize(void)
{
	if (!_php_libxml_initialized) {
		/* we should be the only one's to ever init!! */
		xmlInitParser();

		zend_hash_init(&php_libxml_exports, 0, NULL, NULL, 1);

		_php_libxml_initialized = 1;
	}
}

PHP_LIBXML_API void php_libxml_shutdown(void)
{
	if (_php_libxml_initialized) {
#if defined(LIBXML_SCHEMAS_ENABLED)
		xmlRelaxNGCleanupTypes();
#endif
		xmlCleanupParser();
		zend_hash_destroy(&php_libxml_exports);
		_php_libxml_initialized = 0;
	}
}

PHP_LIBXML_API zval *php_libxml_switch_context(zval *context TSRMLS_DC)
{
	zval *oldcontext;

	oldcontext = LIBXML(stream_context);
	LIBXML(stream_context) = context;
	return oldcontext;

}

static PHP_MINIT_FUNCTION(libxml)
{
	zend_class_entry ce;

	php_libxml_initialize();

	REGISTER_LONG_CONSTANT("LIBXML_VERSION",			LIBXML_VERSION,			CONST_CS | CONST_PERSISTENT);
	REGISTER_STRING_CONSTANT("LIBXML_DOTTED_VERSION",	LIBXML_DOTTED_VERSION,	CONST_CS | CONST_PERSISTENT);
	REGISTER_STRING_CONSTANT("LIBXML_LOADED_VERSION",	(char *)xmlParserVersion,		CONST_CS | CONST_PERSISTENT);

	/* For use with loading xml */
	REGISTER_LONG_CONSTANT("LIBXML_NOENT",		XML_PARSE_NOENT,		CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_DTDLOAD",	XML_PARSE_DTDLOAD,		CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_DTDATTR",	XML_PARSE_DTDATTR,		CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_DTDVALID",	XML_PARSE_DTDVALID,		CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_NOERROR",	XML_PARSE_NOERROR,		CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_NOWARNING",	XML_PARSE_NOWARNING,	CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_NOBLANKS",	XML_PARSE_NOBLANKS,		CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_XINCLUDE",	XML_PARSE_XINCLUDE,		CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_NSCLEAN",	XML_PARSE_NSCLEAN,		CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_NOCDATA",	XML_PARSE_NOCDATA,		CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_NONET",		XML_PARSE_NONET,		CONST_CS | CONST_PERSISTENT);
#if LIBXML_VERSION >= 20621
	REGISTER_LONG_CONSTANT("LIBXML_COMPACT",	XML_PARSE_COMPACT,		CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_NOXMLDECL",	XML_SAVE_NO_DECL,		CONST_CS | CONST_PERSISTENT);
#endif
#if LIBXML_VERSION >= 20703
	REGISTER_LONG_CONSTANT("LIBXML_PARSEHUGE",	XML_PARSE_HUGE,			CONST_CS | CONST_PERSISTENT);
#endif
	REGISTER_LONG_CONSTANT("LIBXML_NOEMPTYTAG",	LIBXML_SAVE_NOEMPTYTAG,	CONST_CS | CONST_PERSISTENT);

	/* Error levels */
	REGISTER_LONG_CONSTANT("LIBXML_ERR_NONE",		XML_ERR_NONE,		CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_ERR_WARNING",	XML_ERR_WARNING,	CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_ERR_ERROR",		XML_ERR_ERROR,		CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("LIBXML_ERR_FATAL",		XML_ERR_FATAL,		CONST_CS | CONST_PERSISTENT);

	INIT_CLASS_ENTRY(ce, "LibXMLError", NULL);
	libxmlerror_class_entry = zend_register_internal_class(&ce TSRMLS_CC);

	if (sapi_module.name) {
		static const char * const supported_sapis[] = {
			"cgi-fcgi",
			"fpm-fcgi",
			"litespeed",
			NULL
		};
		const char * const *sapi_name;

		for (sapi_name = supported_sapis; *sapi_name; sapi_name++) {
			if (strcmp(sapi_module.name, *sapi_name) == 0) {
				_php_libxml_per_request_initialization = 0;
				break;
			}
		}
	}

	if (!_php_libxml_per_request_initialization) {
		/* report errors via handler rather than stderr */
		xmlSetGenericErrorFunc(NULL, php_libxml_error_handler);
		xmlParserInputBufferCreateFilenameDefault(php_libxml_input_buffer_create_filename);
		xmlOutputBufferCreateFilenameDefault(php_libxml_output_buffer_create_filename);
	}

	return SUCCESS;
}


static PHP_RINIT_FUNCTION(libxml)
{
	if (_php_libxml_per_request_initialization) {
		/* report errors via handler rather than stderr */
		xmlSetGenericErrorFunc(NULL, php_libxml_error_handler);
		xmlParserInputBufferCreateFilenameDefault(php_libxml_input_buffer_create_filename);
		xmlOutputBufferCreateFilenameDefault(php_libxml_output_buffer_create_filename);
	}
	return SUCCESS;
}


static PHP_MSHUTDOWN_FUNCTION(libxml)
{
	if (!_php_libxml_per_request_initialization) {
		xmlSetGenericErrorFunc(NULL, NULL);
		xmlSetStructuredErrorFunc(NULL, NULL);

		xmlParserInputBufferCreateFilenameDefault(NULL);
		xmlOutputBufferCreateFilenameDefault(NULL);
	}
	php_libxml_shutdown();

	return SUCCESS;
}


static PHP_RSHUTDOWN_FUNCTION(libxml)
{
	/* reset libxml generic error handling */
	if (_php_libxml_per_request_initialization) {
		xmlSetGenericErrorFunc(NULL, NULL);
		xmlSetStructuredErrorFunc(NULL, NULL);

		xmlParserInputBufferCreateFilenameDefault(NULL);
		xmlOutputBufferCreateFilenameDefault(NULL);
	}

	if (LIBXML(stream_context)) {
		zval_ptr_dtor(&LIBXML(stream_context));
		//prophet generated patch
		{
		int __choose0 = __choose("__SWITCH0");
		if (__choose0 == 0)
		{}
		#ifdef __COMPILE_0
		else if (__choose0 == 1)
		{
		//AddIfStmtKind
		if (!_php_libxml_initialized) {
		    xmlInitParser();
		    _zend_hash_init((&php_libxml_exports), (0), (((void *)0)), (((void *)0)), (1));
		    _php_libxml_initialized = 1;
		}
		}
		#endif
		#ifdef __COMPILE_1
		else if (__choose0 == 2)
		{
		//AddIfStmtKind
		if (!_php_libxml_per_request_initialization) {
		    xmlSetGenericErrorFunc(((void *)0), ((void *)0));
		    xmlSetStructuredErrorFunc(((void *)0), ((void *)0));
		    xmlParserInputBufferCreateFilenameDefault(((void *)0));
		    xmlOutputBufferCreateFilenameDefault(((void *)0));
		}
		}
		#endif
		#ifdef __COMPILE_2
		else if (__choose0 == 3)
		{
		//AddIfStmtKind
		if (!_php_libxml_per_request_initialization) {
		    xmlSetGenericErrorFunc(((void *)0), php_libxml_error_handler);
		    xmlParserInputBufferCreateFilenameDefault(php_libxml_input_buffer_create_filename);
		    xmlOutputBufferCreateFilenameDefault(php_libxml_output_buffer_create_filename);
		}
		}
		#endif
		#ifdef __COMPILE_3
		else if (__choose0 == 4)
		{
		//AddIfStmtKind
		if ((libxml_globals.error_list) == ((void *)0)) {
		    (libxml_globals.error_list) = (zend_llist *)_emalloc((sizeof(zend_llist)));
		    zend_llist_init((libxml_globals.error_list), sizeof(xmlError), (llist_dtor_func_t)_php_libxml_free_error, 0);
		}
		}
		#endif
		#ifdef __COMPILE_4
		else if (__choose0 == 5)
		{
		//AddIfStmtKind
		if ((libxml_globals.error_list)) {
		    zend_llist_clean((libxml_globals.error_list));
		}
		}
		#endif
		#ifdef __COMPILE_5
		else if (__choose0 == 6)
		{
		//AddIfStmtKind
		if ((libxml_globals.error_list)) {
		    zend_llist_destroy((libxml_globals.error_list));
		    _efree(((libxml_globals.error_list)));
		    (libxml_globals.error_list) = ((void *)0);
		}
		}
		#endif
		#ifdef __COMPILE_6
		else if (__choose0 == 7)
		{
		//AddIfStmtKind
		if ((libxml_globals.stream_context)) {
		    _zval_ptr_dtor((&(libxml_globals.stream_context)));
		    (libxml_globals.stream_context) = ((void *)0);
		}
		}
		#endif
		#ifdef __COMPILE_7
		else if (__choose0 == 8)
		{
		//AddIfStmtKind
		if (_php_libxml_initialized) {
		    xmlRelaxNGCleanupTypes();
		    xmlCleanupParser();
		    zend_hash_destroy(&php_libxml_exports);
		    _php_libxml_initialized = 0;
		}
		}
		#endif
		#ifdef __COMPILE_8
		else if (__choose0 == 9)
		{
		//AddIfStmtKind
		if (_php_libxml_per_request_initialization) {
		    xmlSetGenericErrorFunc(((void *)0), ((void *)0));
		    xmlSetStructuredErrorFunc(((void *)0), ((void *)0));
		    xmlParserInputBufferCreateFilenameDefault(((void *)0));
		    xmlOutputBufferCreateFilenameDefault(((void *)0));
		}
		}
		#endif
		#ifdef __COMPILE_9
		else if (__choose0 == 10)
		{
		//AddIfStmtKind
		if (_php_libxml_per_request_initialization) {
		    xmlSetGenericErrorFunc(((void *)0), php_libxml_error_handler);
		    xmlParserInputBufferCreateFilenameDefault(php_libxml_input_buffer_create_filename);
		    xmlOutputBufferCreateFilenameDefault(php_libxml_output_buffer_create_filename);
		}
		}
		#endif
		#ifdef __COMPILE_10
		else if (__choose0 == 11)
		{
		//AddInitKind
		memset(&libxml_globals, 0, sizeof (*(&libxml_globals)));
		}
		#endif
		#ifdef __COMPILE_11
		else if (__choose0 == 12)
		{
		//AddInitKind
		memset(&libxml_globals.error_buffer, 0, sizeof (*(&libxml_globals.error_buffer)));
		}
		#endif
		#ifdef __COMPILE_12
		else if (__choose0 == 13)
		{
		//AddInitKind
		memset(&libxml_globals.error_list, 0, sizeof (*(&libxml_globals.error_list)));
		}
		#endif
		#ifdef __COMPILE_13
		else if (__choose0 == 14)
		{
		//AddInitKind
		memset(&libxml_globals.stream_context, 0, sizeof (*(&libxml_globals.stream_context)));
		}
		#endif
		#ifdef __COMPILE_14
		else if (__choose0 == 15)
		{
		//AddInitKind
		memset(libxml_globals.error_list, 0, sizeof (*(libxml_globals.error_list)));
		}
		#endif
		#ifdef __COMPILE_15
		else if (__choose0 == 16)
		{
		//AddInitKind
		memset(libxml_globals.stream_context, 0, sizeof (*(libxml_globals.stream_context)));
		}
		#endif
		#ifdef __COMPILE_16
		else if (__choose0 == 17)
		{
		//AddStmtAndReplaceAtomKind
		_efree(((libxml_globals && !(1 << 5).error_list)));
		}
		#endif
		#ifdef __COMPILE_17
		else if (__choose0 == 18)
		{
		//AddStmtAndReplaceAtomKind
		_php_libxml_initialized = (0);
		}
		#endif
		#ifdef __COMPILE_18
		else if (__choose0 == 19)
		{
		//AddStmtAndReplaceAtomKind
		_php_libxml_initialized = (module_number);
		}
		#endif
		#ifdef __COMPILE_19
		else if (__choose0 == 20)
		{
		//AddStmtAndReplaceAtomKind
		_php_libxml_initialized = (type);
		}
		#endif
		#ifdef __COMPILE_20
		else if (__choose0 == 21)
		{
		//AddStmtAndReplaceAtomKind
		_php_libxml_per_request_initialization = (0);
		}
		#endif
		#ifdef __COMPILE_21
		else if (__choose0 == 22)
		{
		//AddStmtAndReplaceAtomKind
		_php_libxml_per_request_initialization = (module_number);
		}
		#endif
		#ifdef __COMPILE_22
		else if (__choose0 == 23)
		{
		//AddStmtAndReplaceAtomKind
		_php_libxml_per_request_initialization = (type);
		}
		#endif
		#ifdef __COMPILE_23
		else if (__choose0 == 24)
		{
		//AddStmtAndReplaceAtomKind
		_php_list_set_error_structure(((void *)0), (libxml_globals && !(1 << 5).error_buffer).c);
		}
		#endif
		#ifdef __COMPILE_24
		else if (__choose0 == 25)
		{
		//AddStmtAndReplaceAtomKind
		_zend_hash_init((&php_libxml_exports && !(1 << 5)), (0), (((void *)0)), (((void *)0)), (1));
		}
		#endif
		#ifdef __COMPILE_25
		else if (__choose0 == 26)
		{
		//AddStmtAndReplaceAtomKind
		_zval_ptr_dtor((&(libxml_globals && !(1 << 5).stream_context)));
		}
		#endif
		#ifdef __COMPILE_26
		else if (__choose0 == 27)
		{
		//AddStmtAndReplaceAtomKind
		zend_hash_destroy(&php_libxml_exports && !(1 << 5));
		}
		#endif
		#ifdef __COMPILE_27
		else if (__choose0 == 28)
		{
		//AddStmtAndReplaceAtomKind
		zend_llist_clean((libxml_globals && !(1 << 5).error_list));
		}
		#endif
		#ifdef __COMPILE_28
		else if (__choose0 == 29)
		{
		//AddStmtAndReplaceAtomKind
		zend_llist_destroy((libxml_globals && !(1 << 5).error_list));
		}
		#endif
		#ifdef __COMPILE_29
		else if (__choose0 == 30)
		{
		//AddStmtAndReplaceAtomKind
		zend_llist_init((libxml_globals && !(1 << 5).error_list), sizeof(xmlError), (llist_dtor_func_t)_php_libxml_free_error, 0);
		}
		#endif
		#ifdef __COMPILE_30
		else if (__choose0 == 31)
		{
		//AddStmtKind
		(executor_globals.argument_stack) = zend_vm_stack_new_page(((16 * 1024) - 16));
		}
		#endif
		#ifdef __COMPILE_31
		else if (__choose0 == 32)
		{
		//AddStmtKind
		(libxml_globals.error_list) = ((void *)0);
		}
		#endif
		#ifdef __COMPILE_32
		else if (__choose0 == 33)
		{
		//AddStmtKind
		(libxml_globals.error_list) = (zend_llist *)_emalloc((sizeof(zend_llist)));
		}
		#endif
		#ifdef __COMPILE_33
		else if (__choose0 == 34)
		{
		//AddStmtKind
		(libxml_globals.stream_context) = ((void *)0);
		}
		#endif
		#ifdef __COMPILE_34
		else if (__choose0 == 35)
		{
		//AddStmtKind
		_efree(((libxml_globals.error_list)));
		}
		#endif
		#ifdef __COMPILE_35
		else if (__choose0 == 36)
		{
		//AddStmtKind
		_php_libxml_initialized = 0;
		}
		#endif
		#ifdef __COMPILE_36
		else if (__choose0 == 37)
		{
		//AddStmtKind
		_php_libxml_initialized = 1;
		}
		#endif
		#ifdef __COMPILE_37
		else if (__choose0 == 38)
		{
		//AddStmtKind
		_php_libxml_per_request_initialization = 0;
		}
		#endif
		#ifdef __COMPILE_38
		else if (__choose0 == 39)
		{
		//AddStmtKind
		_php_list_set_error_structure(((void *)0), (libxml_globals.error_buffer).c);
		}
		#endif
		#ifdef __COMPILE_39
		else if (__choose0 == 40)
		{
		//AddStmtKind
		_zend_hash_init((&php_libxml_exports), (0), (((void *)0)), (((void *)0)), (1));
		}
		#endif
		#ifdef __COMPILE_40
		else if (__choose0 == 41)
		{
		//AddStmtKind
		_zval_ptr_dtor((&(libxml_globals.stream_context)));
		}
		#endif
		#ifdef __COMPILE_41
		else if (__choose0 == 42)
		{
		//AddStmtKind
		exit(1);
		}
		#endif
		#ifdef __COMPILE_42
		else if (__choose0 == 43)
		{
		//AddStmtKind
		php_info_print_table_end();
		}
		#endif
		#ifdef __COMPILE_43
		else if (__choose0 == 44)
		{
		//AddStmtKind
		php_info_print_table_row(2, "libXML Compiled Version", "2.7.2");
		}
		#endif
		#ifdef __COMPILE_44
		else if (__choose0 == 45)
		{
		//AddStmtKind
		php_info_print_table_row(2, "libXML Loaded Version", (char *)(*(__xmlParserVersion())));
		}
		#endif
		#ifdef __COMPILE_45
		else if (__choose0 == 46)
		{
		//AddStmtKind
		php_info_print_table_row(2, "libXML streams", "enabled");
		}
		#endif
		#ifdef __COMPILE_46
		else if (__choose0 == 47)
		{
		//AddStmtKind
		php_info_print_table_row(2, "libXML support", "active");
		}
		#endif
		#ifdef __COMPILE_47
		else if (__choose0 == 48)
		{
		//AddStmtKind
		php_info_print_table_start();
		}
		#endif
		#ifdef __COMPILE_48
		else if (__choose0 == 49)
		{
		//AddStmtKind
		php_libxml_initialize();
		}
		#endif
		#ifdef __COMPILE_49
		else if (__choose0 == 50)
		{
		//AddStmtKind
		php_libxml_shutdown();
		}
		#endif
		#ifdef __COMPILE_50
		else if (__choose0 == 51)
		{
		//AddStmtKind
		xmlCleanupParser();
		}
		#endif
		#ifdef __COMPILE_51
		else if (__choose0 == 52)
		{
		//AddStmtKind
		xmlInitParser();
		}
		#endif
		#ifdef __COMPILE_52
		else if (__choose0 == 53)
		{
		//AddStmtKind
		xmlOutputBufferCreateFilenameDefault(((void *)0));
		}
		#endif
		#ifdef __COMPILE_53
		else if (__choose0 == 54)
		{
		//AddStmtKind
		xmlOutputBufferCreateFilenameDefault(php_libxml_output_buffer_create_filename);
		}
		#endif
		#ifdef __COMPILE_54
		else if (__choose0 == 55)
		{
		//AddStmtKind
		xmlParserInputBufferCreateFilenameDefault(((void *)0));
		}
		#endif
		#ifdef __COMPILE_55
		else if (__choose0 == 56)
		{
		//AddStmtKind
		xmlParserInputBufferCreateFilenameDefault(php_libxml_input_buffer_create_filename);
		}
		#endif
		#ifdef __COMPILE_56
		else if (__choose0 == 57)
		{
		//AddStmtKind
		xmlRelaxNGCleanupTypes();
		}
		#endif
		#ifdef __COMPILE_57
		else if (__choose0 == 58)
		{
		//AddStmtKind
		xmlResetLastError();
		}
		#endif
		#ifdef __COMPILE_58
		else if (__choose0 == 59)
		{
		//AddStmtKind
		xmlSetGenericErrorFunc(((void *)0), ((void *)0));
		}
		#endif
		#ifdef __COMPILE_59
		else if (__choose0 == 60)
		{
		//AddStmtKind
		xmlSetGenericErrorFunc(((void *)0), php_libxml_error_handler);
		}
		#endif
		#ifdef __COMPILE_60
		else if (__choose0 == 61)
		{
		//AddStmtKind
		xmlSetStructuredErrorFunc(((void *)0), ((void *)0));
		}
		#endif
		#ifdef __COMPILE_61
		else if (__choose0 == 62)
		{
		//AddStmtKind
		xmlSetStructuredErrorFunc(((void *)0), php_libxml_structured_error_handler);
		}
		#endif
		#ifdef __COMPILE_62
		else if (__choose0 == 63)
		{
		//AddStmtKind
		zend_error((1 << 3L), "A non well formed numeric value encountered");
		}
		#endif
		#ifdef __COMPILE_63
		else if (__choose0 == 64)
		{
		//AddStmtKind
		zend_hash_destroy(&php_libxml_exports);
		}
		#endif
		#ifdef __COMPILE_64
		else if (__choose0 == 65)
		{
		//AddStmtKind
		zend_llist_clean((libxml_globals.error_list));
		}
		#endif
		#ifdef __COMPILE_65
		else if (__choose0 == 66)
		{
		//AddStmtKind
		zend_llist_destroy((libxml_globals.error_list));
		}
		#endif
		#ifdef __COMPILE_66
		else if (__choose0 == 67)
		{
		//AddStmtKind
		zend_llist_init((libxml_globals.error_list), sizeof(xmlError), (llist_dtor_func_t)_php_libxml_free_error, 0);
		}
		#endif
		#ifdef __COMPILE_67
		else if (__choose0 == 68)
		{
		//IfExitKind
		if (__is_neg("0-68", "L710", 5, &(libxml_globals.stream_context), sizeof (libxml_globals.stream_context), &(libxml_globals.stream_context),
					 sizeof (libxml_globals.stream_context), &(libxml_globals.error_list), sizeof (libxml_globals.error_list), &(libxml_globals.error_list), sizeof (libxml_globals.error_list), &(_php_libxml_per_request_initialization),
					 sizeof (_php_libxml_per_request_initialization)))
		    return 0;
		}
		#endif
		int __choose1 = __choose("__SWITCH1");
		if (__choose1 == 0)
		{
		LIBXML(stream_context) = NULL;
		
		}
		#ifdef __COMPILE_68
		else if (__choose1 == 1)
		{
		//GuardKind
		if (!__is_neg("1-1", "L710", 5, &(libxml_globals.stream_context), sizeof (libxml_globals.stream_context), &(libxml_globals.stream_context), sizeof (libxml_globals.stream_context), &(libxml_globals.error_list), sizeof (libxml_globals.error_list), &(libxml_globals.error_list), sizeof (libxml_globals.error_list), &(_php_libxml_per_request_initialization), sizeof (_php_libxml_per_request_initialization)))
		    (libxml_globals.stream_context) = ((void *)0);
		
		}
		#endif
		#ifdef __COMPILE_69
		else if (__choose1 == 2)
		{
		//MSVExtRemoveStmtKind
		{
		}
		
		}
		#endif
		#ifdef __COMPILE_70
		else if (__choose1 == 3)
		{
		//ReplaceKind
		(libxml_globals.stream_context) = ((void *)0);
		
		}
		#endif
		}
	}
	smart_str_free(&LIBXML(error_buffer));
	if (LIBXML(error_list)) {
		zend_llist_destroy(LIBXML(error_list));
		efree(LIBXML(error_list));
		LIBXML(error_list) = NULL;
	}
	xmlResetLastError();

	return SUCCESS;
}


static PHP_MINFO_FUNCTION(libxml)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "libXML support", "active");
	php_info_print_table_row(2, "libXML Compiled Version", LIBXML_DOTTED_VERSION);
	php_info_print_table_row(2, "libXML Loaded Version", (char *)xmlParserVersion);
	php_info_print_table_row(2, "libXML streams", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ proto void libxml_set_streams_context(resource streams_context) 
   Set the streams context for the next libxml document load or write */
static PHP_FUNCTION(libxml_set_streams_context)
{
	zval *arg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &arg) == FAILURE) {
		return;
	}
	if (LIBXML(stream_context)) {
		zval_ptr_dtor(&LIBXML(stream_context));
		LIBXML(stream_context) = NULL;
	}
	Z_ADDREF_P(arg);
	LIBXML(stream_context) = arg;
}
/* }}} */

/* {{{ proto bool libxml_use_internal_errors([boolean use_errors]) 
   Disable libxml errors and allow user to fetch error information as needed */
static PHP_FUNCTION(libxml_use_internal_errors)
{
	xmlStructuredErrorFunc current_handler;
	zend_bool use_errors=0, retval;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &use_errors) == FAILURE) {
		return;
	}

	current_handler = xmlStructuredError;
	if (current_handler && current_handler == php_libxml_structured_error_handler) {
		retval = 1;
	} else {
		retval = 0;
	}

	if (ZEND_NUM_ARGS() == 0) {
		RETURN_BOOL(retval);
	}

	if (use_errors == 0) {
		xmlSetStructuredErrorFunc(NULL, NULL);
		if (LIBXML(error_list)) {
			zend_llist_destroy(LIBXML(error_list));
			efree(LIBXML(error_list));
			LIBXML(error_list) = NULL;
		}
	} else {
		xmlSetStructuredErrorFunc(NULL, php_libxml_structured_error_handler);
		if (LIBXML(error_list) == NULL) {
			LIBXML(error_list) = (zend_llist *) emalloc(sizeof(zend_llist));
			zend_llist_init(LIBXML(error_list), sizeof(xmlError), (llist_dtor_func_t) _php_libxml_free_error, 0);
		}
	}
	RETURN_BOOL(retval);
}
/* }}} */

/* {{{ proto object libxml_get_last_error() 
   Retrieve last error from libxml */
static PHP_FUNCTION(libxml_get_last_error)
{
	xmlErrorPtr error;

	error = xmlGetLastError();
	
	if (error) {
		object_init_ex(return_value, libxmlerror_class_entry);
		add_property_long(return_value, "level", error->level);
		add_property_long(return_value, "code", error->code);
		add_property_long(return_value, "column", error->int2);
		if (error->message) {
			add_property_string(return_value, "message", error->message, 1);
		} else {
			add_property_stringl(return_value, "message", "", 0, 1);
		}
		if (error->file) {
			add_property_string(return_value, "file", error->file, 1);
		} else {
			add_property_stringl(return_value, "file", "", 0, 1);
		}
		add_property_long(return_value, "line", error->line);
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto object libxml_get_errors()
   Retrieve array of errors */
static PHP_FUNCTION(libxml_get_errors)
{
	
	xmlErrorPtr error;

	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}

	if (LIBXML(error_list)) {

		error = zend_llist_get_first(LIBXML(error_list));

		while (error != NULL) {
			zval *z_error;
			MAKE_STD_ZVAL(z_error);

			object_init_ex(z_error, libxmlerror_class_entry);
			add_property_long(z_error, "level", error->level);
			add_property_long(z_error, "code", error->code);
			add_property_long(z_error, "column", error->int2);
			if (error->message) {
				add_property_string(z_error, "message", error->message, 1);
			} else {
				add_property_stringl(z_error, "message", "", 0, 1);
			}
			if (error->file) {
				add_property_string(z_error, "file", error->file, 1);
			} else {
				add_property_stringl(z_error, "file", "", 0, 1);
			}
			add_property_long(z_error, "line", error->line);
			add_next_index_zval(return_value, z_error);

			error = zend_llist_get_next(LIBXML(error_list));
		}
	}
}
/* }}} */

/* {{{ proto void libxml_clear_errors() 
   Clear last error from libxml */
static PHP_FUNCTION(libxml_clear_errors)
{
	xmlResetLastError();
	if (LIBXML(error_list)) {
		zend_llist_clean(LIBXML(error_list));
	}
}
/* }}} */

/* {{{ proto bool libxml_disable_entity_loader([boolean disable]) 
   Disable/Enable ability to load external entities */
static PHP_FUNCTION(libxml_disable_entity_loader)
{
	zend_bool disable = 1;
	xmlParserInputBufferCreateFilenameFunc old;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|b", &disable) == FAILURE) {
		return;
	}

	if (disable == 0) {
		old = xmlParserInputBufferCreateFilenameDefault(php_libxml_input_buffer_create_filename);
	} else {
		old = xmlParserInputBufferCreateFilenameDefault(php_libxml_input_buffer_noload);
	}

	if (old == php_libxml_input_buffer_noload) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}
/* }}} */

/* {{{ Common functions shared by extensions */
int php_libxml_xmlCheckUTF8(const unsigned char *s)
{
	int i;
	unsigned char c;

	for (i = 0; (c = s[i++]);) {
		if ((c & 0x80) == 0) {
		} else if ((c & 0xe0) == 0xc0) {
			if ((s[i++] & 0xc0) != 0x80) {
				return 0;
			}
		} else if ((c & 0xf0) == 0xe0) {
			if ((s[i++] & 0xc0) != 0x80 || (s[i++] & 0xc0) != 0x80) {
				return 0;
			}
		} else if ((c & 0xf8) == 0xf0) {
			if ((s[i++] & 0xc0) != 0x80 || (s[i++] & 0xc0) != 0x80 || (s[i++] & 0xc0) != 0x80) {
				return 0;
			}
		} else {
			return 0;
		}
	}
	return 1;
}

int php_libxml_register_export(zend_class_entry *ce, php_libxml_export_node export_function)
{
	php_libxml_func_handler export_hnd;
	
	/* Initialize in case this module hasnt been loaded yet */
	php_libxml_initialize();
	export_hnd.export_func = export_function;

	return zend_hash_add(&php_libxml_exports, ce->name, ce->name_length + 1, &export_hnd, sizeof(export_hnd), NULL);
}

PHP_LIBXML_API xmlNodePtr php_libxml_import_node(zval *object TSRMLS_DC)
{
	zend_class_entry *ce = NULL;
	xmlNodePtr node = NULL;
	php_libxml_func_handler *export_hnd;

	if (object->type == IS_OBJECT) {
		ce = Z_OBJCE_P(object);
		while (ce->parent != NULL) {
			ce = ce->parent;
		}
		if (zend_hash_find(&php_libxml_exports, ce->name, ce->name_length + 1, (void **) &export_hnd)  == SUCCESS) {
			node = export_hnd->export_func(object TSRMLS_CC);
		}
	}
	return node;
}

PHP_LIBXML_API int php_libxml_increment_node_ptr(php_libxml_node_object *object, xmlNodePtr node, void *private_data TSRMLS_DC)
{
	int ret_refcount = -1;

	if (object != NULL && node != NULL) {
		if (object->node != NULL) {
			if (object->node->node == node) {
				return object->node->refcount;
			} else {
				php_libxml_decrement_node_ptr(object TSRMLS_CC);
			}
		}
		if (node->_private != NULL) {
			object->node = node->_private;
			ret_refcount = ++object->node->refcount;
			/* Only dom uses _private */
			if (object->node->_private == NULL) {
				object->node->_private = private_data;
			}
		} else {
			ret_refcount = 1;
			object->node = emalloc(sizeof(php_libxml_node_ptr));
			object->node->node = node;
			object->node->refcount = 1;
			object->node->_private = private_data;
			node->_private = object->node;
		}
	}

	return ret_refcount;
}

PHP_LIBXML_API int php_libxml_decrement_node_ptr(php_libxml_node_object *object TSRMLS_DC)
{
	int ret_refcount = -1;
	php_libxml_node_ptr *obj_node;

	if (object != NULL && object->node != NULL) {
		obj_node = (php_libxml_node_ptr *) object->node;
		ret_refcount = --obj_node->refcount;
		if (ret_refcount == 0) {
			if (obj_node->node != NULL) {
				obj_node->node->_private = NULL;
			}
			efree(obj_node);
		} 
		object->node = NULL;
	}

	return ret_refcount;
}

PHP_LIBXML_API int php_libxml_increment_doc_ref(php_libxml_node_object *object, xmlDocPtr docp TSRMLS_DC)
{
	int ret_refcount = -1;

	if (object->document != NULL) {
		object->document->refcount++;
		ret_refcount = object->document->refcount;
	} else if (docp != NULL) {
		ret_refcount = 1;
		object->document = emalloc(sizeof(php_libxml_ref_obj));
		object->document->ptr = docp;
		object->document->refcount = ret_refcount;
		object->document->doc_props = NULL;
	}

	return ret_refcount;
}

PHP_LIBXML_API int php_libxml_decrement_doc_ref(php_libxml_node_object *object TSRMLS_DC)
{
	int ret_refcount = -1;

	if (object != NULL && object->document != NULL) {
		ret_refcount = --object->document->refcount;
		if (ret_refcount == 0) {
			if (object->document->ptr != NULL) {
				xmlFreeDoc((xmlDoc *) object->document->ptr);
			}
			if (object->document->doc_props != NULL) {
				if (object->document->doc_props->classmap) {
					zend_hash_destroy(object->document->doc_props->classmap);
					FREE_HASHTABLE(object->document->doc_props->classmap);
				}
				efree(object->document->doc_props);
			}
			efree(object->document);
			object->document = NULL;
		}
	}

	return ret_refcount;
}

PHP_LIBXML_API void php_libxml_node_free_resource(xmlNodePtr node TSRMLS_DC)
{
	if (!node) {
		return;
	}

	switch (node->type) {
		case XML_DOCUMENT_NODE:
		case XML_HTML_DOCUMENT_NODE:
			break;
		default:
			if (node->parent == NULL || node->type == XML_NAMESPACE_DECL) {
				php_libxml_node_free_list((xmlNodePtr) node->children TSRMLS_CC);
				switch (node->type) {
					/* Skip property freeing for the following types */
					case XML_ATTRIBUTE_DECL:
					case XML_DTD_NODE:
					case XML_DOCUMENT_TYPE_NODE:
					case XML_ENTITY_DECL:
					case XML_ATTRIBUTE_NODE:
					case XML_NAMESPACE_DECL:
					case XML_TEXT_NODE:
						break;
					default:
						php_libxml_node_free_list((xmlNodePtr) node->properties TSRMLS_CC);
				}
				if (php_libxml_unregister_node(node TSRMLS_CC) == 0) {
					node->doc = NULL;
				}
				php_libxml_node_free(node);
			} else {
				php_libxml_unregister_node(node TSRMLS_CC);
			}
	}
}

PHP_LIBXML_API void php_libxml_node_decrement_resource(php_libxml_node_object *object TSRMLS_DC)
{
	int ret_refcount = -1;
	xmlNodePtr nodep;
	php_libxml_node_ptr *obj_node;

	if (object != NULL && object->node != NULL) {
		obj_node = (php_libxml_node_ptr *) object->node;
		nodep = object->node->node;
		ret_refcount = php_libxml_decrement_node_ptr(object TSRMLS_CC);
		if (ret_refcount == 0) {
			php_libxml_node_free_resource(nodep TSRMLS_CC);
		} else {
			if (obj_node && object == obj_node->_private) {
				obj_node->_private = NULL;
			}
		}
	}
	if (object != NULL && object->document != NULL) {
		/* Safe to call as if the resource were freed then doc pointer is NULL */
		php_libxml_decrement_doc_ref(object TSRMLS_CC);
	}
}
/* }}} */

#ifdef PHP_WIN32
PHP_LIBXML_API BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return xmlDllMain(hinstDLL, fdwReason, lpvReserved);
}
#endif

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
