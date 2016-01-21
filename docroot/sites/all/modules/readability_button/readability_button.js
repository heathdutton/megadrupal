(function() {
var s     = document.getElementsByTagName('script')[0],
    rdb   = document.createElement('script');
rdb.type  = 'text/javascript';
rdb.async = true;
rdb.src   = document.location.protocol + '//www.readability.com/embed.js';
s.parentNode.insertBefore(rdb, s);
})();
  