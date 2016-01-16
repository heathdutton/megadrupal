/**
 * @file
 * Provides support for URL rewriting using JavaScript
 *
 * Rewriting URLs with JavaScript is less preferred. Not only must a user
 * have JavaScript enabled, but no major errors can result during the loading
 * of the JavaScript files. Caching can also influence whether these files are
 * loaded.
 *
 * The best way to rewrite URLs is to patch Drupal to use the default URL
 * rewriting functions on the server-side
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */
$(document).ready(function() {
  Drupal.settings.ezproxy_url_rewrite.links = new Array();

  Drupal.settings.ezproxy_url_rewrite.url_alter = function() {
    var links = $("a");
    var settings = Drupal.settings.ezproxy_url_rewrite;

    // Set parsing of URLs to strict mode
    parseUri.options.strictMode = true;

    // Parse each URL and determine whether it is an internal or external
    // depending on whether a hostname has been parsed
    for (var i = 0; i < links.length; i++) {
      var link = links[i];
      var obj = $(link);
      var url = parseUri(obj.attr("href"));

      url.external = url.host ? true : false;
      Drupal.settings.ezproxy_url_rewrite.links.push(url);

      // External or internal link
      if ((url.external && !settings.url_type.external) ||
         (!url.external && !settings.url_type.internal)) {
        continue;
      }

      // Regular expression pattern matching against parts of the URL
      var protocol_regex = new RegExp(settings.protocol_regex.replace('\\', '\\\\'), "i");
      var host_regex = new RegExp(settings.host_regex.replace('\\', '\\\\'), "i");
      var port_regex = new RegExp(settings.port_regex.replace('\\', '\\\\'), "i");
      var path_regex = new RegExp(settings.path_regex.replace('\\', '\\\\'), "i");
      var query_string_regex = new RegExp(settings.query_string_regex.replace('\\', '\\\\'), "i");
      var fragment_regex = new RegExp(settings.fragment_regex.replace('\\', '\\\\'), "i");

      if ((!settings.protocol_regex && !url.protocol.match(protocol_regex))
          || (!settings.host_regex && !url.host.match(host_regex))
          || (!settings.port_regex && !url.port.match(port_regex))
          || (!settings.path_regex && !url.path.match(path_regex))
          || (!settings.query_string_regex && !url.query.match(query_string_regex))
          || (!settings.fragment_regex && !url.anchor.match(fragment_regex))) {
        continue;
      }

      var prefix = settings.prefix;
      var infix = settings.infix;
      var suffix = settings.suffix;

      // We made it this far, now do the rewriting
      var absolute = prefix + (url.protocol ? url.protocol + '://' : '') + url.host + infix + (url.port ? ':' + url.port : '') + url.path + (url.query ? '?' + url.query : '') + (url.anchor ? '#' + url.anchor : '') + suffix;
      obj.attr("href", absolute);
    }
  }

  // If URL rewriting is enabled and active and JavaScript is enabled,
  // then do the rewriting
  if (Drupal.settings.ezproxy_url_rewrite.enabled && Drupal.settings.ezproxy_url_rewrite.active
        && Drupal.settings.ezproxy_url_rewrite.javascript_enabled) {
    Drupal.settings.ezproxy_url_rewrite.url_alter();
  }

});
