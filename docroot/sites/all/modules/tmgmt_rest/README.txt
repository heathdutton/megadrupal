Overview
-------------------------------
TMGMT REST module is a plugin for Translation Management Tool module.
It calls / creates custom REST services for automated translation of the
content.
This module can be used when an external translation provider has
the ability/flexibility to work with the custom REST services.

Features
-------------------------------
The TMGMT REST module needs an external REST endpoint and secret key setting
for the translator. When creating a translation job, the module does a POST request
with the timestamp, job id and XLIFF xml to the endpoint set for the translator.
For authorization we create a sha256 encrypted signature based on the POST data.

The module also creates a custom REST endpoint so the external translation
provider can POST back the translated data. This endpoint will also require
a POST request with a timestamp, job id and XLIFF xml signed with a sha256
encrypted signature. The custom endpoint can be found at: translation/api/rest

More information on the API can be found below.

Requirements
-------------------------------
This module requires TMGMT (http://drupal.org/project/tmgmt) module and the TMGMT
Export / Import File submodule to be installed.

External API
-------------------------------
The external translation provider needs a REST endpoint that receives the
following POST data.
- timestamp: Unix timestamp of this request.
- jobid: The job id in Drupal used to validate the data when it is sent back.
- xlf: XML in XLIFF format.

The POST request will contain an Authorization header with a signature to
validate the request. The following steps are needed to validate the request.
- Concatenate the timestamp, the jobid en the xlf data with a pipe (|) character like:
  <timestamp>|<jobid>|<xlf-data>
- sha256 encrypt the string with the secret key set in the tmgmt translator settings.
- Base64 encode the result.
- The external service can check if the timestamp is not older then 5 minutes to
  prevent abuse.
- Header example of a request:
  POST
    /translation/api/rest HTTP/1.0
    Content-Type: application/x-www-form-urlencoded
    Authorization: JEMoJfKOJ62d_vA0PaU28M_F1qG4GmVhVzeAoMZnN0Y

Internal API
-------------------------------
After the data is translated the translation provider can POST back the xml to
our custom REST endpoint: translation/api/rest.
This endpoint works the same way as the external webservice should. It receives the
following POST data.
- timestamp: Unix timestamp of this request.
- jobid: The job id in Drupal used to validate the data when it is sent back.
- xlf: XML in XLIFF format.

The request must contain an Authorization header with a signature to validate the
request (see the rules for the external API to create a valid signature).

For valid requests the endpoint will return a http status 200 OK. If the translation
can not be saved, we return the following http status codes to provide information
about the error.
- 405 Method Not Allowed: The request is not a POST.
- 408 Request Timeout: The timestamp in the request is more then 5 minutes ago.
- 401 Not Authorized: The jobid and/or signature are not valid.
- 400 Bad Request: The XLIFF data is not valid or the Job ID does not correspond with
  the XLIFF XML.
- 500 Internal error: Unknown error, the Drupal logs need to be investigated.