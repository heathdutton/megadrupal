jQuery(document).ready(
    function() {
        var tealium_script = document.createElement("script");
        tealium_script.src = Drupal.settings.tealium.tealium_script;
        tealium_script.type = "text/javascript";
        var first_script = document.getElementsByTagName("script")[0];
        first_script.parentNode.insertBefore(first_script, tealium_script);
    }
);
