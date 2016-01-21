Create a file called "detect-device.vcl" and copy the following vcl logic.
Ensure this file is included in your own vcl using, for example;

include "/etc/varnish/detect-device.vcl";

This is all you need to do as each function is then appended to the functions
in your own vcl file.

################################################################################

sub vcl_recv {
  call identify_device;
}

// Set a device grouping based on the accessing user-agent.
sub identify_device {
  // Set a default of "NA" which will be used if the user-agent isn't one of the
  // mobile groups.
  set req.http.X-Device = "NA";

  // Check user-agents
  if (req.http.User-Agent ~ "iPad") {
    set req.http.X-Device = "mobile-tablet";
  }
  elsif (req.http.User-Agent ~ "iP(hone|od)" || req.http.User-Agent ~ "Android" ) {
    set req.http.X-Device = "mobile-smart";
  }
  elsif (req.http.User-Agent ~ "SymbianOS" || req.http.User-Agent ~ "^(BlackBerry|LG|Nokia|SonyEricsson|SAMSUNG)") {
    set req.http.X-Device = "mobile-other";
  }
}

sub vcl_hash {
  if (req.http.X-Device ~ "^mobile") {
    // Varnish 2.x => 3.x compatibility.
    // Swap the lines below if you're using Varnish 2.x.
    // set req.hash += req.http.X-Device;
    hash_data(req.http.X-Device);
  }
}
