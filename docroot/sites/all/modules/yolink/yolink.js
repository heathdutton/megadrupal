function getAPIKey(email) {
    $.ajax({
        url : 'https://admin.yolink.com:8443/yoadmin/register-external-user?o=jsonp&p=yolink.com&e='+email,
        data : {},
        dataType : 'jsonp',
        success: function(data){
            if (data.apikey) {
                $('#edit-yolink-api-key').val(data.apikey);
            }
            else {
                alert("Hm. No API key was generated. Please visit http://yolink.com/yolink/pricing to get a key.");
            }
        }
    });
}