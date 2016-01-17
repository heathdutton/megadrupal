sub vcl_recv {
  // HAG sites where we brutally cache everything
  // this probably breaks logins and so on
  //if (req.http.host ~ "cache0?[0-9]?.hahaha.com" || !req.http.host) {
  //  set req.backend = default;
  //} else {
    set req.backend = aegirvps;
  //}

  // configuration from https://wiki.fourkitchens.com/display/PF/Configure+Varnish+for+Pressflow
  if (req.request != "GET" &&
    req.request != "HEAD" &&
    req.request != "PUT" &&
    req.request != "POST" &&
    req.request != "TRACE" &&
    req.request != "OPTIONS" &&
    req.request != "DELETE") {
      /* Non-RFC2616 or CONNECT which is weird. */
      return (pipe);
  }

  if (req.request != "GET" && req.request != "HEAD") {
    /* We only deal with GET and HEAD by default */
    return (pass);
  }

  if (req.http.Cookie) {
    // Remove has_js and Google Analytics cookies.
    set req.http.Cookie = regsuball(req.http.Cookie, "(^|;\s*)(__[a-z]+|__utma_a2a|has_js)=[^;]*", "");

    // more removal, from: https://www.varnish-cache.org/trac/wiki/VCLExampleRemovingSomeCookies

    // remove other stats cookies
    set req.http.Cookie = regsuball(req.http.Cookie, "(^|; ) *__utm.=[^;]+;? *", "\1"); # removes all cookies named __utm? (utma, utmb...) - tracking thing

    // Remove all cookies but sessions
    set req.http.Cookie = ";" req.http.Cookie;
    set req.http.Cookie = regsuball(req.http.Cookie, "; +", ";");
    set req.http.Cookie = regsuball(req.http.Cookie, ";(SESS[a-zA-Z0-9=]+)=", "; \1=");
    set req.http.Cookie = regsuball(req.http.Cookie, ";[^ ][^;]*", "");
    set req.http.Cookie = regsuball(req.http.Cookie, "^[; ]+|[; ]+$", "");

    // Remove a ";" prefix, if present.
    set req.http.Cookie = regsub(req.http.Cookie, "^;\s*", "");

    // Remove empty cookies.
    if (req.http.Cookie ~ "^\s*$") {
      unset req.http.Cookie;
    }
  }
  
  if (req.http.Authorization || req.http.Cookie) {
    /* Not cacheable by default */
    return (pass);
  }

  // Skip the Varnish cache for install, update, and cron
  if (req.url ~ "install\.php|update\.php|cron\.php") {
    return (pass);
  }

  if (req.url ~ "d7ad077a69c611e08c7a3f5cd3215ad5") {
    purge_url (".");
    error 500 "the cache is now purged.";
  }


  // Normalize the Accept-Encoding header
  // as per: http://varnish-cache.org/wiki/FAQ/Compression
  if (req.http.Accept-Encoding) {
    if (req.url ~ "\.(jpg|png|gif|gz|tgz|bz2|tbz|mp3|ogg)$") {
      # No point in compressing these
      remove req.http.Accept-Encoding;
    }
    elsif (req.http.Accept-Encoding ~ "gzip") {
      set req.http.Accept-Encoding = "gzip";
    }
    else {
      # Unknown or deflate algorithm
      remove req.http.Accept-Encoding;
    }
  }

  // Let's have a little grace
  set req.grace = 30s;

  return (lookup);
}

// configuration from https://wiki.fourkitchens.com/display/PF/Configure+Varnish+for+Pressflow
sub vcl_hash {
	if (req.http.Cookie) {
		set req.hash += req.http.Cookie;
	}
}

// Strip any cookies before an image/js/css is inserted into cache.
// configuration from https://wiki.fourkitchens.com/display/PF/Configure+Varnish+for+Pressflow
sub vcl_fetch {
 if (req.url ~ "\.(png|gif|jpg|swf|css|js)$") {
   // For Varnish 2.1 or later, replace obj with beresp:
   unset beresp.http.set-cookie;
 }
}
