
{mask:main}
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="{language}" xml:lang="{language}">

<head>
  <title>{head_title}</title>
  {head}
  {styles}
  {scripts}
</head>

<body>
<table border="0" cellpadding="0" cellspacing="0" id="header">
  <tr>
    <td id="logo">
    	{if: {logo} }<a href="{base_path}"><img src="{logo}"/></a>{/if}
      {if: {site_name} }<h1 class='site-name'><a href="{base_path}">{site_name}</a></h1>{/if}
      {if: {site_slogan} }<div class='site-slogan'>{site_slogan}</div>{/if}
    </td>
    <td id="search_box">
      {search_box}
    </td>
  </tr>
  <tr>
    <td colspan="2"><div>{header}</div></td>
  </tr>
</table>

<table border="0" cellpadding="0" cellspacing="0" id="content">
  <tr>
    {if: {left} }<td valign="top" id="sidebar-left">
      {left}
    </td>{/if}
    <td valign="top">
      {if: {mission} }<div id="mission">{mission}</div>{/if}
      <div id="main">
        {breadcrumb}
        <h1 class="title">{title}</h1>
        <div class="tabs">{tabs}</div>
        {help}
        {messages}
        {content}
        {feed_icons}
      </div>
    </td>
    {if: {sidebar_right} }<td id="sidebar-right">
      {sidebar_right}
    </td>{/if}
  </tr>
</table>

<div id="footer">
  {footer_message}
</div>
{closure}
</body>
</html>
{/mask}
