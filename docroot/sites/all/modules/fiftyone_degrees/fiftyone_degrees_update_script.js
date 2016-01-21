fiftyone_degrees_start_updates();
_request.onload = updatePendingFinish;

function updatePendingFinish() {
    if(_request.responseText === undefined || _request.responseText.length == 0) {
        var message = 'The update procedure could not be launched, probably because the server does not support flushing.<br />';
        message += 'The update can be downloaded manually from ';
        message += '<a href=\"https://51degrees.mobi/Products/Downloads/Premium.aspx?LicenseKeys=' + getUpdateKey() + '&Type=WordPress&Download=True\">51Degrees.mobi</a>.';
    }
    else {
        var message = 'Updating has finished.';
    }

    document.getElementById('finished_message').innerHTML = message;
}
