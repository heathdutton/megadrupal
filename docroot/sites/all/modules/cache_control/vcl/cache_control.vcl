# Default VCL for Drupal Cache Control module
# Exove / Erno Kaikkonen
# VCL ver. 2.2.0
# Varnish 2.1.x required
 
backend public {
  .host = "192.0.2.1";
  .port = "80";
  .connect_timeout = 5s;
  .first_byte_timeout = 60s;
  .between_bytes_timeout = 30s;
}

# The Drupal admin interface has a few places where requests can take a long 
# time to complete. This sets longer timeout values for those URLs.
backend admin {
  .host = "192.0.2.1";
  .port = "80";
  .connect_timeout = 300s;
  .first_byte_timeout = 300s;
  .between_bytes_timeout = 300s;
}
 
# Only allow PURGE requests from the system itself.
acl purge {
  "127.0.0.1";
  "10.0.0.0"/8;
  "192.168.0.0"/16;
}

sub vcl_recv {
  # See "backend admin" above  
  if (req.url ~ "install.php" || req.url ~ "update.php" || req.url ~ "cron.php" || req.url ~ "^/admin") {
    set req.backend = admin;
    return (pass);
  }
  else {
    set req.backend = public;  
  }

  # See "acl purge" above
  if (req.request == "PURGE") {
    if (!client.ip ~ purge) {
      error 405 "Not allowed.";
    }
    
    # Changed to allow wildcard purging for CC's session hashed URLs
    purge("req.http.host == " req.http.host " && req.url ~ " req.url "$"); 
    #purge("req.url == " req.url " && req.http.host == " req.http.host);
    
    error 200 "Purged.";
  }
  
  # Just in case something accessed via IP-addresses gets cached incorrectly
  if ((!req.http.host) || (req.http.host == "")) {
    return (pass);
  }
  
  # See cache_control.module:_cache_control_set_cookie();
  if (req.http.Cookie ~ "cacheControlDisabled=1") {
    return (pass);
  }
 
  if (req.request == "OPTIONS" || req.request == "POST" || req.request == "PUT") {
    return (pass);
  }
   
  if (req.request == "GET" || req.request == "HEAD") {
    return (lookup);
  }
 
  error 405;
}
 
sub vcl_hash {
  # Cache Control defaults:
  if (req.http.Accept-Encoding ~ "gzip") {
    set req.hash += "gzip";
  } 
  else if (req.http.Accept-Encoding ~ "deflate") {
    set req.hash += "deflate";
  }
}

sub vcl_fetch {

  # Prevent the client from caching the content; Everything is always served
  # "Varnish-fresh". This way PURGE requests immediately make the new content
  # available to everyone.
  #
  # Add debug cache headers so we know what Varnish is doing. This list of 
  # headers corresponds to the headers sent by 
  # cache_control.module:cache_control_send_cache_headers() and 
  # cache_control.module:cache_control_send_no_cache_headers() .
  
  set beresp.http.X-DRCC-Pragma = beresp.http.Pragma;
  set beresp.http.X-DRCC-Last-Modified = beresp.http.Last-Modified;
  set beresp.http.X-DRCC-Cache-Control = beresp.http.Cache-Control;
  set beresp.http.X-DRCC-Expires = beresp.http.Expires;

  # Remove caching headers
  unset beresp.http.Last-Modified;
  unset beresp.http.Expires;
  unset beresp.http.Cache-Control;

  # Add no-cache headers instead
  set beresp.http.Pragma = "no-cache";
  set beresp.http.Cache-Control = "no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0";
  set beresp.http.Expires = "Sun, 03 Jan 1971 00:00:00 GMT";
  

  # Just in case something accessed via IP-addresses gets cached incorrectly
  if ((!bereq.http.host) || (bereq.http.host == "")) {
      return (pass);
  }

  # Make sure content behind passwords doesn't get cached
  if (beresp.http.Authenticate || bereq.http.Authorization) {
    return (pass);
  }

  # See cache_control.module:_cache_control_set_cookie();
  if (bereq.http.Cookie ~ "cacheControlDisabled=1") {
    return (pass);
  }
  
  if (bereq.request == "OPTIONS" || bereq.request == "POST" || bereq.request == "PUT") {
     return (pass);
  }
  
  if (bereq.request == "GET" || bereq.request == "HEAD") {
    # If user just logged out, pass the unauthenticated cookie to a client
    # without caching the content.
    if ((bereq.http.Cookie ~ "cacheControlAuthenticated=1") && (beresp.http.Set-Cookie ~ "cacheControlAuthenticated=0")) {
      return (pass);
    }
 
    # If the user just logged in, pass, since the user needs his cc auth cookie.
    if (beresp.http.Set-Cookie ~ "cacheControlAuthenticated=1") {
      return (pass);
    }
 
    # Otherwise strip all cookies and cache the response. We don't want to 
    # leave any cookies in the cached content.
    unset beresp.http.Set-Cookie;
    return (deliver);
  }
 
  error 405;
 
}

sub vcl_deliver {
  if (obj.hits > 0) {
    set resp.http.X-Cache = "HIT";
  } else {
    set resp.http.X-Cache = "MISS";
  }
}
