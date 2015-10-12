sub vcl_error {
  // Let's deliver a friendlier error page.
  // You can customize this as you wish.
  set obj.http.Content-Type = "text/html; charset=utf-8";
  synthetic {"
  <?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<title>"} obj.status " " obj.response {"</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
      <link rel="stylesheet" type="text/css" charset="utf-8" media="screen" href="http://filet.koumbit.net/koumbit.css">
    </head>
    <body>
<div id="wrapper">
<div id="banniere">
<img src="http://filet.koumbit.net/banniere.png" alt="Koumbit.org" />
</div>

    <div id="contenu">
<div style="float: right;"><img src="http://filet.koumbit.net/503not.png" alt="Error 503 / Erreur 503"></div>

    <h1 id="header"><div id="logo">Service unavailable | service non-disponible</div></h1>
<h2>English</h2>
<p><strong>Please wait a few seconds and try again.</strong></p>

<p>If you are reading this page, it means we either have a major operation
underway or an unexpected overload or downtime. See <a
href="http://identi.ca/group/koumbitsysadmin">our server status on
Identi.ca</a> or write to support@koumbit.org.</p>

<p>This page will reload automatically in 15 seconds.</p>

<h2>Fran&ccedil;ais</h2>
<p><strong>Veuillez attendre un moment puis r&eacute;-essayer.</strong></p>

<p>Si vous lisez ceci, c'est que notre r&eacute;seau subit une op&eacute;ration
majeure, une surcharge ou une coupure inattendue. Les op&eacute;rations
majeures sont annonc&eacute;es sur <a
href="http://identi.ca/group/koumbitstatus">identi.ca</a>. Si vous n'y
trouvez pas d'informations sur l'interruption actuelle, alors il s'agit
fort probablement d'une micro-coupure de quelques secondes. En ce cas,il
vous suffit de recharger la page; la situation devrait se r&eacute;soudre
d'elle-m&eacute;me.</p>

<p>Si le probl&egrave;me persiste, nous vous invitons &agrave; <a
href="mailto:support@koumbit.org" title="Panne de r&eacute;seau">rejoindre
notre service de support technique.</a></p>

<p>Une nouvelle tentative sera faite dans 15 secondes.</p>

    <hr />     <h4>Debug Info:</h4>
    <pre>
Status: "} obj.status {"
Response: "} obj.response {"
XID: "} req.xid {"
</pre>
      <address><a href="http://www.varnish-cache.org/">Varnish</a></address>
<div style="clear: both;"></div>

      </div>
</div>
    </body>
   </html>
   "};
    // For Varnish 2.1 or later, replace deliver with return(deliver):
    return(deliver);
//   deliver;
}
