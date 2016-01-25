Installation
  1. Download.
  2. Enable.
  3. For Varnish, copy this to your default.vcl at the end of sub vcl_fetch.
    if (beresp.status >= 200 && beresp.status < 400 &&
        (req.request == "PUT" ||
         req.request == "POST" ||
         req.request == "DELETE")
       ){
        ban("obj.http.Xache-Contains ~ " + beresp.http.Xache-Remove);
    }
  4. Clear cache for all pages just one time. If you have a module for that, 
     you already know how to do it. If you are a command line enthusiast, 
     do this
        varnishadm -T :6082 "ban.url /"
     where 6082 is the default Varnish port number.
  5. Done and done. Now, cache for pages will be removed if and when they are 
     updated. Automatically. That's the beauty of Xache - plug-it and 
     forget-it.

Other than Varnish
It can work with other caching servers too. You may follow the instructions in 
the step 3 above to rewrite the code for your own caching server. Please share 
with me if you do, so it can be of help to others too.
