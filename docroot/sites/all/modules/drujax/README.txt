Drujax
======
Drupal ajax module.

Drujax adds a seperate content.tpl.php file which is rendered on ajax requests 
without html.tpl.php and page.tpl.php. Everything stays the same 
on normal page requests.

Drujax makes use of jquery address push states. Every content link will 
automaticly load your page content through ajax. 

Documentation
=============

js
--

### Custom ajax complete Handler

    <script>
        Drujax.setHandler(yourHandler);// calls yourHandler(json,path)
    </script>

Example:

    <script>
        Drujax.setHandler(function(data,path){
            $("#drujax-main").animate({
                marginLeft:-$(window).width()
              },
              500,
              function(){
                for(var i in data.content){
                 $(i).html(data.content[i]);
                }
                $(this).animate({marginLeft:0},500);
            });
        })
    </script>

php
---

### Add variables to the json reponse
    
    <?php 
        // In your content.tpl.php
        drujax_var("key", "value");
    ?>

this json response will look like this
    
    {
        "title":"page title",
        "key":"value",
        "content":{
            "#drujax-main":"your content.tpl.php output"
        }
    }
