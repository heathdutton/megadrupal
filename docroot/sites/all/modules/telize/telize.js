(function($) {
    // custom Drupal behaviors here
    function callTelize() {
        $.getJSON("http://www.telize.com/geoip",
                function(json) {
                    for (var key in telize_redirect_urls) {
                        if (telize_redirect_urls.hasOwnProperty(key)) {
                            console.log(json.country_code);
                            console.log(key);
                            if (json.country_code == key) {
                                document.location = telize_redirect_urls[key];
                            }
                        }
                    }
                }
        );
    }
    $(document).ready(callTelize);

})(jQuery);