
CERTIFY

A module for issuing PDF certificates upon completion of quizzes from the Quiz
module and books from the book module.

Developed by the Norwegian Center for integrated care and Telemedicine.

REQUIREMENTS

- The tool pdftk installed somewhere on the server
  (available from http://www.pdfhacks.com/pdftk/ ).
- Drupal 6.x
- Quiz module >= v4.0
- Book module
- PHP5

HOW TO INSTALL

1. Upload and extract to your modules directory as a normal Drupal module.
2. Install pdftk on the server.
3. Create a directory for storage of certificates and templates. This should be
   a directory outside of your webroot, both readable and writable by the web
   server.
4. Enable the module and configure it using the paths to your pdftk executable
   and the certificate directory.

HOW TO USE

1. Create content of type certificate.
    - Mark which quizzes are required to pass in order to receive the
      certificate.
    - (OPTIONAL) Upload a certificate template in PDF form format. Fields
      available:
      * 'site_name' (Site name)
      * 'full_name' (Full name of user)
      * 'email' (User email address)
      * 'certificate_name' (Certificate title)
      * 'certificate_desc' (Certificate description)
      * 'score' (Score)
      * 'issued_date' (Date)
2. Let users take tests.
3. Users will have certificates available for download upon completion.
4. ???
5. Profit!

OTHER STUFF

- Certificate templates are one for each node, so all revisions will share the same template.
- Certificate templates may be made using Adobe Acrobat or Open Office. Skribe has also been reported as working for this purpose.

KNOW BUGS

None.
