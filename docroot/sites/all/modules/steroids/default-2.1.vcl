################################################################################
# Varnish version 2.1
#
# This is the default vcl file you can start to build your own config.
# You HAVE to change the cookie values to match you steroids admin settings
# Look for the sections marked with CHANGE_THIS_FOR_YOUR_SETUP_!!!
#
################################################################################

backend default {
    .host = "127.0.0.1";
    .port = "80";
}

################################################################################
# Custom subroutines to provide ESI support for Drupal blocks with the
# Drupal ESI module.
################################################################################

# Customise the hash if required.
sub esi_block__hash {
}

sub esi_block__recv {
}

sub esi_block__fetch {
  # ESI blocks with per-user or per-role config have a cache-control: private
  # header.  This removes the header and inserts the block into the cache.
  if( req.http.Cache-Control ~ "private" ) {
    unset req.http.Set-Cookie;
    unset req.http.Cache-Control;
    return(deliver);
  }
}

################################################################################
### END ESI ###
################################################################################

# vcl_recv
# --------
# Called at the beginning of a request, after the complete request
# has been received and parsed.  Its purpose is to decide whether
# or not to serve the request, how to do it, and, if applicable,
# which backend to use.

sub vcl_recv {
  // Allow the backend to serve up stale content if it is responding slowly.
  set req.grace = 6h;

  // Use anonymous, cached pages if all backends are down.
  # if (!req.backend.healthy) {
  #  unset req.http.Cookie;
  # }

  // Always cache the following file types for all users.
  if (req.url ~ "(?i)\.(png|gif|jpeg|jpg|ico|swf|css|js|html|htm)(\?[a-z0-9]+)?$") {
    unset req.http.Cookie;
  }

  // Pipe through request types
  if (req.request != "GET" &&
    req.request != "HEAD" &&
    req.request != "PUT" &&
    req.request != "POST" &&
    req.request != "TRACE" &&
    req.request != "OPTIONS" &&
    req.request != "DELETE") {
      return (pipe);
  }

  if (req.request != "GET" && req.request != "HEAD") {
    return (pass);
  }

  // CHANGE_THIS_FOR_YOUR_SETUP_!!!
  // Skip caching if the cache bypass cookie is set (see steroids settings page)
  if (req.http.Cookie ~ "vcache-bypass-token=pohceciKue3Qua3ooV3kookieb9Veix7") {
    return(pass);
  }

  // Skip the Varnish cache for install, update, cron, logout and user pages
  if (req.url ~ "^/install\.php|^/update\.php|^/cron\.php") {
    return (pass);
  }

  // Remove has_js and Google Analytics cookies.
  set req.http.Cookie = regsuball(req.http.Cookie, "(^|;\s*)(__[a-z]+)=[^;]*", "");
  // Remove a ";" prefix, if present.
  set req.http.Cookie = regsub(req.http.Cookie, "^;\s*", "");
  // Remove empty cookies.
  if (req.http.Cookie ~ "^\s*$") {
    unset req.http.Cookie;
  }

  // Handle browser encoding.
  if (req.http.Accept-Encoding) {
    if (req.url ~ "\.(jpg|jpeg|png|gif|gz|tgz|bz2|tbz|mp3|ogg|swf|mp4|flv)$") {
      # don't try to compress already compressed files
      remove req.http.Accept-Encoding;
    } elsif (req.http.Accept-Encoding ~ "gzip") {
      set req.http.Accept-Encoding = "gzip";
    } elsif (req.http.Accept-Encoding ~ "deflate") {
      set req.http.Accept-Encoding = "deflate";
    } else {
      # unkown algorithm
      remove req.http.Pragma;
      remove req.http.Cache-Control;
    }
    remove req.http.Accept-Encoding;
  }

  // ESI blocks
  call esi_block__recv;

  return (lookup);
}

# vcl_hash
# ---------
#

sub vcl_hash {
  call esi_block__hash;
}

# vcl_fetch
# ---------
# Called after a document has been successfully retrieved from the
# backend.

sub vcl_fetch {
  // Strip any cookies before an image/js/css is inserted into cache.
  if (req.url ~ "\.(png|gif|jpg|swf|css|js)$") {
    unset beresp.http.Set-Cookie;
  }

  //Do not allow Set-Cookie if not needed to protect the cache
  //Allow Set-Cookie if vcache-token is a mismatch with the drupal session cookie
  if (! req.url ~ "(login|logout|user|users)" && ! beresp.http.X-Varnish-Cookie ~ "Allow") {
    remove beresp.http.Set-Cookie;
  }

  // Don't ESI anything with a 3/4 letter extension
  // (e.g. don't try to ESI images, css, etc).
  if (! req.url ~ "\..{3,4}$") {
   esi;
  }

  // Allow a grace period for offering "stale" data in case backend lags
  // Exeptional long grace time to prevent Cache error pages
  set req.grace = 72h;

 // ESI blocks
  call esi_block__fetch;

}

# vcl_deliver
# -----------
# Called before a cached object is delivered to the client.

sub vcl_deliver {

  // Show hit or miss in header response.
  if (obj.hits > 0) {
    set resp.http.X-Cache = "HIT";
    set resp.http.X-Cache-Hits = obj.hits;
  }
  else {
    set resp.http.X-Cache = "MISS";
  }

  // CHANGE_THIS_FOR_YOUR_SETUP_!!!
  // Skip caching if the cache bypass cookie is set (see steroids settings page)
  if (req.http.Cookie ~ "vcache-bypass-token=pohceciKue3Qua3ooV3kookieb9Veix7") {
    set resp.http.X-Admin = "Yes - Steroids cache-bypass active";
  }

  // Show backend health status in header response.
  if (req.backend.healthy) {
    set resp.http.X-Back = "Ok";
  }
  else {
    set resp.http.X-Back = "N.Ok";
  }
}

# vcl_pass
# --------
# Called upon entering pass mode.  In this mode, the request is
# passed on to the backend, and the backend's response is passed on
# to the client, but is not entered into the cache.  Subsequent
# requests submitted over the same client connection are handled
# normally.

# sub vcl_pass {}


# vcl_hit
# -------
# Called after a cache lookup if the requested document was found
# in the cache.

# sub vcl_hit {}


# vcl_miss
# --------
# Called after a cache lookup if the requested document was not
# found in the cache.  Its purpose is to decide whether or not to
# attempt to retrieve the document from the backend, and which
# backend to use.

# sub vcl_miss {}


