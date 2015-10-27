$(document).ready(function() {
   /*$("a.message_docs_href").click(function() {
     message_docs_redirect($(this));
     return false;
   });*/
});

function message_docs_redirect(my_object) {
  //alert('1'+my_object.attr('message_text'));
  $.ajax({
    type: "POST",
    url: my_object.attr('href'),
    data: "message="+my_object.attr('message_text'),
    success: function(msg){
      //alert(msg);
      window.location.replace(msg);
    }
  });
}