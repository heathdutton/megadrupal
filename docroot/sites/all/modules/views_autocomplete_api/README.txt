Usage :

Views autocomplete API allows you to create autocomplete path via Views.

How to use it :

1 - Create a View :
>  The last field of the view is used to show the autocomplete options.
>  The previous one is used to define what will be sent to the form when
selecting.
>  The exposed input filters will be used with the user input. You can use OR / AND.
>  You can use the header for permanent.

2 - On any textfield form element, just do a form alter to have 

"#autocomplete_path" => 'views_autocomplete_api/[the machine name of your view 1]/[the machine name of your view 2]/etc..'

Note that the master display of the Views will be used (you can show it in the advanced options of Views).

3 - If needed, you can always alter the result set by using hook_autocomplete_api_alter.

// TODO : 
>  example module.
>  Use the "empty" element of Views (if set) when no result are available.
>  Views permission (vital).