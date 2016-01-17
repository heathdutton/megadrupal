<?php print "<?xml"; ?> version="1.0" encoding="UTF-8"<?php print "?>\n"; ?>
<catalog
  xmlns="http://xcri.org/profiles/1.2/catalog"
  xmlns:xcriTerms="http://xcri.org/profiles/catalog/terms"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xmlns:xhtml="http://www.w3.org/1999/xhtml"
  xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:dcterms="http://purl.org/dc/terms/"
  xmlns:credit="http://purl.org/net/cm"
  xmlns:mlo="http://purl.org/net/mlo"
  xmlns:courseDataProgramme="http://xcri.co.uk"
  xsi:schemaLocation="http://xcri.org/profiles/1.2/catalog http://www.xcri.co.uk/bindings/xcri_cap_1_2.xsd http://xcri.org/profiles/1.2/catalog/terms  http://www.xcri.co.uk/bindings/xcri_cap_terms_1_2.xsd http://xcri.co.uk http://www.xcri.co.uk/bindings/coursedataprogramme.xsd"
  generated="<?php print $generated ?>">

  <?php if ($contributor): ?>
    <dc:contributor><?php print $contributor; ?></dc:contributor>
  <?php endif; // contributor ?>

  <?php if ($description): ?>
    <dc:description>
      <div xmlns="http://www.w3.org/1999/xhtml">
        <?php print $description; ?>
      </div>
    </dc:description>
  <?php endif; // description ?>

  <?php foreach ($providers as $provider): ?>
    <provider>

      <?php if (!empty($provider['description'])): ?>
        <dc:description>
          <div xmlns="http://www.w3.org/1999/xhtml">
            <?php print $provider['description']; ?>
          </div>
        </dc:description>
      <?php endif; // description ?>

      <dc:identifier><?php print $provider['identifier']; ?></dc:identifier>

      <dc:title><?php print $provider['title']; ?></dc:title>

      <mlo:url><?php print $provider['url']; ?></mlo:url>

      <?php foreach ($provider['courses'] as $course): ?>
        <course>

          <?php if (!empty($course['description'])): ?>
            <dc:description>
              <div xmlns="http://www.w3.org/1999/xhtml">
                <?php print $course['description']; ?>
              </div>
            </dc:description>
          <?php endif; ?>

          <dc:identifier><?php print $course['identifier']; ?></dc:identifier>

          <?php if (!empty($course['subjects'])): ?>
            <?php foreach ($course['subjects'] as $subject): ?>
              <dc:subject><?php print $subject ?></dc:subject>
            <?php endforeach; // subjects ?>
          <?php endif; ?>

          <dc:title><?php print $course['title']; ?></dc:title>

          <?php if (!empty($course['type'])): ?>
            <dc:type><?php print $course['type']; ?></dc:type>
          <?php endif; ?>

          <mlo:url><?php print $course['url']; ?></mlo:url>

          <?php if (!empty($course['abstract'])): ?>
            <abstract><?php print $course['abstract']; ?></abstract>
          <?php endif; ?>

          <?php if (!empty($course['application_procedure'])): ?>
            <applicationProcedure>
              <div xmlns="http://www.w3.org/1999/xhtml">
                <?php print $course['application_procedure']; ?>
              </div>
            </applicationProcedure>
          <?php endif; ?>

          <?php if (!empty($course['assessment'])): ?>
            <mlo:assessment>
              <div xmlns="http://www.w3.org/1999/xhtml">
                <?php print $course['assessment']; ?>
              </div>
            </mlo:assessment>
          <?php endif; ?>

          <?php if (!empty($course['learning_outcome'])): ?>
            <learningOutcome>
              <div xmlns="http://www.w3.org/1999/xhtml">
                <?php print $course['learning_outcome']; ?>
              </div>
            </learningOutcome>
          <?php endif; ?>

          <?php if (!empty($course['objective'])): ?>
            <mlo:objective>
              <div xmlns="http://www.w3.org/1999/xhtml">
                <?php print $course['objective']; ?>
              </div>
            </mlo:objective>
          <?php endif; ?>

          <?php if (!empty($course['prerequisite'])): ?>
            <mlo:prerequisite>
              <div xmlns="http://www.w3.org/1999/xhtml">
                <?php print $course['prerequisite']; ?>
              </div>
            </mlo:prerequisite>
          <?php endif; ?>

          <?php if (!empty($course['regulations'])): ?>
            <regulations>
              <div xmlns="http://www.w3.org/1999/xhtml">
                <?php print $course['regulations']; ?>
              </div>
            </regulations>
          <?php endif; ?>

          <?php foreach ($course['qualifications'] as $qualification): ?>
            <mlo:qualification>

              <dc:identifier><?php print $qualification['identifier']; ?></dc:identifier>

              <dc:title><?php print $qualification['title']; ?></dc:title>

              <?php if (!empty($qualification['abbr'])): ?>
                <abbr><?php print $qualification['abbr']; ?></abbr>
              <?php endif; ?>

              <?php if (!empty($qualification['description'])): ?>
                <dc:description>
                  <div xmlns="http://www.w3.org/1999/xhtml">
                    <?php print $qualification['description']; ?>
                  </div>
                </dc:description>
              <?php endif; ?>

              <?php if (!empty($qualification['education_level'])): ?>
                <dcterms:educationLevel><?php print $qualification['education_level']; ?></dcterms:educationLevel>
              <?php endif; ?>

              <?php if (!empty($qualification['awarded_by'])): ?>
                <awardedBy><?php print $qualification['awarded_by']; ?></awardedBy>
              <?php endif; ?>

              <?php if (!empty($qualification['accredited_by'])): ?>
                <accreditedBy><?php print $qualification['accredited_by']; ?></accreditedBy>
              <?php endif; ?>

            </mlo:qualification>
          <?php endforeach; // qualifications ?>

          <?php foreach ($course['credits'] as $credit): ?>
            <mlo:credit>
              <?php if (!empty($credit['scheme'])): ?>
                <credit:scheme><?php print $credit['scheme']; ?></credit:scheme>
              <?php endif; ?>
              <credit:level><?php print $credit['level']; ?></credit:level>
              <credit:value><?php print $credit['value']; ?></credit:value>
            </mlo:credit>
          <?php endforeach; // credits?>

          <?php foreach ($course['presentations'] as $presentation): ?>
            <presentation>

              <?php if (!empty($presentation['description'])): ?>
                <dc:description>
                  <div xmlns="http://www.w3.org/1999/xhtml">
                    <?php print $presentation['description']; ?>
                  </div>
                </dc:description>
              <?php endif; ?>

              <dc:identifier><?php print $presentation['identifier']; ?></dc:identifier>

              <?php if (!empty($presentation['subjects'])): ?>
                <?php foreach ($presentation['subjects'] as $subject): ?>
                  <dc:subject><?php print $subject ?></dc:subject>
                <?php endforeach; // subjects ?>
              <?php endif; ?>

              <dc:title><?php print $presentation['title']; ?></dc:title>

              <?php if (!empty($presentation['abstract'])): ?>
                <abstract><?php print $presentation['abstract']; ?></abstract>
              <?php endif; ?>

              <?php if (!empty($presentation['application_procedure'])): ?>
                <applicationProcedure>
                  <div xmlns="http://www.w3.org/1999/xhtml">
                    <?php print $presentation['application_procedure']; ?>
                  </div>
                </applicationProcedure>
              <?php endif; ?>

              <?php if (!empty($presentation['assessment'])): ?>
                <mlo:assessment>
                  <div xmlns="http://www.w3.org/1999/xhtml">
                    <?php print $presentation['assessment']; ?>
                  </div>
                </mlo:assessment>
              <?php endif; ?>

              <?php if (!empty($presentation['learning_outcome'])): ?>
                <learningOutcome>
                  <div xmlns="http://www.w3.org/1999/xhtml">
                    <?php print $presentation['learning_outcome']; ?>
                  </div>
                </learningOutcome>
              <?php endif; ?>

              <?php if (!empty($presentation['objective'])): ?>
                <mlo:objective>
                  <div xmlns="http://www.w3.org/1999/xhtml">
                    <?php print $presentation['objective']; ?>
                  </div>
                </mlo:objective>
              <?php endif; ?>

              <?php if (!empty($presentation['prerequisite'])): ?>
                <mlo:prerequisite>
                  <div xmlns="http://www.w3.org/1999/xhtml">
                    <?php print $presentation['prerequisite']; ?>
                  </div>
                </mlo:prerequisite>
              <?php endif; ?>

              <?php if (!empty($presentation['regulations'])): ?>
                <regulations>
                  <div xmlns="http://www.w3.org/1999/xhtml">
                    <?php print $presentation['regulations']; ?>
                  </div>
                </regulations>
              <?php endif; ?>

              <?php if (!empty($presentation['start'])): ?>
                <mlo:start><?php print $presentation['start']; ?></mlo:start>
              <?php endif; ?>

              <?php if (!empty($presentation['end'])): ?>
                <end><?php print $presentation['end']; ?></end>
              <?php endif; ?>

              <?php if (!empty($presentation['duration'])): ?>
                <mlo:duration><?php print $presentation['duration']; ?></mlo:duration>
              <?php endif; ?>

              <?php if (!empty($presentation['apply_from'])): ?>
                <applyFrom><?php print $presentation['apply_from']; ?></applyFrom>
              <?php endif; ?>

              <?php if (!empty($presentation['apply_until'])): ?>
                <applyUntil><?php print $presentation['apply_until']; ?></applyUntil>
              <?php endif; ?>

              <?php if (!empty($presentation['apply_to'])): ?>
                <applyTo><?php print $presentation['apply_to']; ?></applyTo>
              <?php endif; ?>

              <?php if (!empty($presentation['study_mode_id']) && !empty($presentation['study_mode'])): ?>
                <studyMode identifier="<?php print $presentation['study_mode_id']; ?>"><?php print $presentation['study_mode']; ?></studyMode>
              <?php endif; ?>

              <?php if (!empty($presentation['attendance_mode_id']) && !empty($presentation['attendance_mode'])): ?>
                <attendanceMode identifier="<?php print $presentation['attendance_mode_id']; ?>"><?php print $presentation['attendance_mode']; ?></attendanceMode>
              <?php endif; ?>

              <?php if (!empty($presentation['attendance_pattern_id']) && !empty($presentation['attendance_pattern'])): ?>
                <attendancePattern identifier="<?php print $presentation['attendance_pattern_id']; ?>"><?php print $presentation['attendance_pattern']; ?></attendancePattern>
              <?php endif; ?>

              <?php if (!empty($presentation['language_of_instruction_id']) && !empty($presentation['language_of_instruction'])): ?>
                <mlo:languageOfInstruction><?php print $presentation['language_of_instruction_id']; ?></mlo:languageOfInstruction>
              <?php endif; ?>

              <?php if (!empty($presentation['language_of_assessment_id']) && !empty($presentation['language_of_assessment'])): ?>
                <languageOfAssessment><?php print $presentation['language_of_assessment_id']; ?></languageOfAssessment>
              <?php endif; ?>

              <?php if (!empty($presentation['places'])): ?>
                <mlo:places><?php print $presentation['places']; ?></mlo:places>
              <?php endif; ?>

              <?php if (!empty($presentation['cost'])): ?>
                <mlo:cost><?php print $presentation['cost']; ?></mlo:cost>
              <?php endif; ?>

              <?php if (!empty($presentation['age'])): ?>
                <age><?php print $presentation['age']; ?></age>
              <?php endif; ?>

              <?php foreach ($presentation['venues'] as $venue): ?>
                <venue>
                  <provider>

                    <?php if (!empty($venue['description'])): ?>
                      <dc:description>
                        <div xmlns="http://www.w3.org/1999/xhtml">
                          <?php print $venue['description']; ?>
                        </div>
                      </dc:description>
                    <?php endif; ?>

                    <dc:identifier><?php print $venue['identifier']; ?></dc:identifier>

                    <dc:title><?php print $venue['title']; ?></dc:title>

                    <mlo:location>
                      <?php if (!empty($venue['location']['town'])): ?>
                        <mlo:town><?php print $venue['location']['town']; ?></mlo:town>
                      <?php endif; ?>

                      <?php if (!empty($venue['location']['postcode'])): ?>
                        <mlo:postcode><?php print $venue['location']['postcode']; ?></mlo:postcode>
                      <?php endif; ?>

                      <mlo:address><?php print $venue['location']['address']; ?></mlo:address>

                      <?php if (!empty($venue['location']['phone'])): ?>
                        <mlo:phone><?php print $venue['location']['phone']; ?></mlo:phone>
                      <?php endif; ?>

                      <?php if (!empty($venue['location']['fax'])): ?>
                        <mlo:fax><?php print $venue['location']['fax']; ?></mlo:fax>
                      <?php endif; ?>

                      <?php if (!empty($venue['location']['email'])): ?>
                        <mlo:email><?php print $venue['location']['email']; ?></mlo:email>
                      <?php endif; ?>

                      <?php if (!empty($venue['location']['url'])): ?>
                        <mlo:url><?php print $venue['location']['url']; ?></mlo:url>
                      <?php endif; ?>

                    </mlo:location>

                  </provider>
                </venue>
              <?php endforeach; // venues ?>

            </presentation>
          <?php endforeach; // presentations ?>

        </course>
      <?php endforeach; // courses ?>

      <mlo:location>
        <?php if (!empty($provider['location']['town'])): ?>
          <mlo:town><?php print $provider['location']['town']; ?></mlo:town>
        <?php endif; ?>

        <?php if (!empty($provider['location']['postcode'])): ?>
          <mlo:postcode><?php print $provider['location']['postcode']; ?></mlo:postcode>
        <?php endif; ?>

        <mlo:address><?php print $provider['location']['address']; ?></mlo:address>

        <?php if (!empty($provider['location']['phone'])): ?>
          <mlo:phone><?php print $provider['location']['phone']; ?></mlo:phone>
        <?php endif; ?>

        <?php if (!empty($provider['location']['fax'])): ?>
          <mlo:fax><?php print $provider['location']['fax']; ?></mlo:fax>
        <?php endif; ?>

        <?php if (!empty($provider['location']['email'])): ?>
          <mlo:email><?php print $provider['location']['email']; ?></mlo:email>
        <?php endif; ?>

        <?php if (!empty($provider['location']['url'])): ?>
          <mlo:url><?php print $provider['location']['url']; ?></mlo:url>
        <?php endif; ?>

      </mlo:location>

    </provider>
  <?php endforeach; // provider ?>

</catalog>
