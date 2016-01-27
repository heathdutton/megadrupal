<?php

/*
 * @file
 * Contains the SalsaAPI test class.
 */

/**
 * Service class used for tests.
 */
class SalsaAPIMock extends SalsaAPI implements SalsaAPIInterface {
  public function query($script, $query) {
    $query = explode('&', $query);
    $response = FALSE;
    if (($query[0] == 'object=supporter') && isset($query[1]) &&
      (($query[1] == 'condition=supporter_KEY=1') || ($query[1] == 'condition=Email=example%40example.com'))) {
      $response = <<<EOF
<?xml version="1.0"?>
  <data organization_KEY="11111">
    <supporter>
      <item>
        <supporter_KEY>1</supporter_KEY>
        <organization_KEY>11111</organization_KEY>
        <chapter_KEY/>
        <Last_Modified>Mon May 21 2012 11:56:46 GMT-0400 (EDT)</Last_Modified>
        <Date_Created>Mon May 20 2012 16:48:23 GMT-0400 (EDT)</Date_Created>
        <Title>Mr</Title>
        <First_Name>First Name</First_Name>
        <MI/>
        <Last_Name>Last Name</Last_Name>
        <Suffix/>
        <Email>example@example.com</Email>
        <Password>d41d8cd98f00b204e9800998ecf8427e</Password>
        <Receive_Email>1</Receive_Email>
        <Email_Status/>
        <Email_Preference/>
        <Soft_Bounce_Count>0</Soft_Bounce_Count>
        <Hard_Bounce_Count>0</Hard_Bounce_Count>
        <Last_Bounce/>
        <Receive_Phone_Blasts_BOOLVALUE>false</Receive_Phone_Blasts_BOOLVALUE>
        <Receive_Phone_Blasts>false</Receive_Phone_Blasts>
        <Phone/>
        <Cell_Phone/>
        <Phone_Provider/>
        <Work_Phone/>
        <Pager/>
        <Home_Fax/>
        <Work_Fax/>
        <Street>Example Street</Street>
        <Street_2/>
        <Street_3/>
        <City>Example City</City>
        <State/>
        <Zip>1000</Zip>
        <PRIVATE_Zip_Plus_4>0000</PRIVATE_Zip_Plus_4>
        <County/>
        <District/>
        <Country>CH</Country>
        <Latitude>0.000000</Latitude>
        <Longitude>0.000000</Longitude>
        <Organization/>
        <Department/>
        <Occupation/>
        <Instant_Messenger_Service/>
        <Instant_Messenger_Name/>
        <Web_Page/>
        <Alternative_Email/>
        <Other_Data_1/>
        <Other_Data_2/>
        <Other_Data_3/>
        <Notes/>
        <Source>Web</Source>
        <Source_Details>(No Referring info)</Source_Details>
        <Source_Tracking_Code>(No Original Source Available)</Source_Tracking_Code>
        <Tracking_Code/>
        <Status/>
        <Timezone/>
        <Language_Code/>
        <uid/>
        <custom_field_1/>
        <custom_field_2/>
        <key>1</key>
        <object>supporter</object>
      </item>
    <count>1</count>
  </supporter>
</data>
EOF;
    }
    elseif ($query[0] == 'object=signup_page') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <signup_page>
    <item>
      <signup_page_KEY>1</signup_page_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY>0</chapter_KEY>
      <Reference_Name>Signup Page</Reference_Name>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <Date_Created>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Date_Created>
      <Style>Normal</Style>
      <Title>Signup Page</Title>
      <Header>Header - The content at the top of the page</Header>
      <Request>First_Name,Last_Name,Email,Street,City,Zip,custom_field_1,custom_field_2</Request>
      <Required>,First_Name,Last_Name,Email</Required>
      <Include_Remember_Me_Link_BOOLVALUE>false</Include_Remember_Me_Link_BOOLVALUE>
      <Include_Remember_Me_Link>false</Include_Remember_Me_Link>
      <Opt_in_Text>Required Confirmation Text</Opt_in_Text>
      <PreFooter>Content before the Submit button</PreFooter>
      <Footer>Content after the submit button</Footer>
      <PreGroup_Text>Text before the group selection</PreGroup_Text>
      <groups_KEYS>0,1,2,3,</groups_KEYS>
      <Automatically_add_to_Groups_BOOLVALUE>false</Automatically_add_to_Groups_BOOLVALUE>
      <Automatically_add_to_Groups>false</Automatically_add_to_Groups>
      <PreInterest_Text>Pre-tag text - Text to display before the checkboxes for optional tags</PreInterest_Text>
      <interest_KEYS/>
      <tag_KEYS>1,2,3</tag_KEYS>
      <event_KEY>0</event_KEY>
      <Request_Volunteer_Availability_BOOLVALUE>false</Request_Volunteer_Availability_BOOLVALUE>
      <Request_Volunteer_Availability>false</Request_Volunteer_Availability>
      <Email_Confirmed_Opt_In_BOOLVALUE>false</Email_Confirmed_Opt_In_BOOLVALUE>
      <Email_Confirmed_Opt_In>false</Email_Confirmed_Opt_In>
      <Default_Tracking_Code/>
      <redirect_path/>
      <Maximum_Field_Width>0</Maximum_Field_Width>
      <PRIVATE_trigger/>
      <email_trigger_KEYS/>
      <READONLY_Hit_Count/>
      <chapter_lookup/>
      <Template/>
      <add_to_groups_KEYS>0,1,</add_to_groups_KEYS>
      <optionally_add_to_groups_KEYS>0,1,2,3,</optionally_add_to_groups_KEYS>
      <key>1</key>
      <object>signup_page</object>
    </item>
    <count>1</count>
  </signup_page>
</data>
EOF;
    }
    elseif ($query[0] == 'object=groups' && count($query) == 1) {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <groups>
    <item>
      <groups_KEY>1</groups_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY>0</chapter_KEY>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <Date_Created>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Date_Created>
      <Group_Name>Group 1</Group_Name>
      <Reference_Name/>
      <parent_KEY/>
      <Description/>
      <Notes/>
      <Display_To_User_BOOLVALUE>false</Display_To_User_BOOLVALUE>
      <Display_To_User>false</Display_To_User>
      <Listserve_Type/>
      <Subscription_Type/>
      <Manager/>
      <Moderator_Emails/>
      <Subject_Prefix/>
      <Listserve_Responses/>
      <Append_Header/>
      <Append_Footer/>
      <Custom_Headers/>
      <Listserve_Options/>
      <external_ID/>
      <From_Email/>
      <From_Name/>
      <Reply_To/>
      <Headers_To_Remove/>
      <Confirmation_Message/>
      <Auto_Update_BOOLVALUE>false</Auto_Update_BOOLVALUE>
      <Auto_Update>false</Auto_Update>
      <query_KEY/>
      <Smart_Group_Options/>
      <Smart_Group_Error/>
      <enable_statistics_BOOLVALUE>false</enable_statistics_BOOLVALUE>
      <enable_statistics>false</enable_statistics>
      <join_email_trigger_KEYS/>
      <key>1</key>
      <object>groups</object>
    </item>
    <item>
      <groups_KEY>2</groups_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY>0</chapter_KEY>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <Date_Created>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Date_Created>
      <Group_Name>Group 2</Group_Name>
      <Reference_Name/>
      <parent_KEY/>
      <Description/>
      <Notes/>
      <Display_To_User_BOOLVALUE>false</Display_To_User_BOOLVALUE>
      <Display_To_User>false</Display_To_User>
      <Listserve_Type/>
      <Subscription_Type/>
      <Manager/>
      <Moderator_Emails/>
      <Subject_Prefix/>
      <Listserve_Responses/>
      <Append_Header/>
      <Append_Footer/>
      <Custom_Headers/>
      <Listserve_Options/>
      <external_ID/>
      <From_Email/>
      <From_Name/>
      <Reply_To/>
      <Headers_To_Remove/>
      <Confirmation_Message/>
      <Auto_Update_BOOLVALUE>false</Auto_Update_BOOLVALUE>
      <Auto_Update>false</Auto_Update>
      <query_KEY/>
      <Smart_Group_Options/>
      <Smart_Group_Error/>
      <enable_statistics_BOOLVALUE>false</enable_statistics_BOOLVALUE>
      <enable_statistics>false</enable_statistics>
      <join_email_trigger_KEYS/>
      <key>2</key>
      <object>groups</object>
    </item>
    <item>
      <groups_KEY>3</groups_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY>0</chapter_KEY>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <Date_Created>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Date_Created>
      <Group_Name>Group 3</Group_Name>
      <Reference_Name/>
      <parent_KEY/>
      <Description/>
      <Notes/>
      <Display_To_User_BOOLVALUE>false</Display_To_User_BOOLVALUE>
      <Display_To_User>false</Display_To_User>
      <Listserve_Type/>
      <Subscription_Type/>
      <Manager/>
      <Moderator_Emails/>
      <Subject_Prefix/>
      <Listserve_Responses/>
      <Append_Header/>
      <Append_Footer/>
      <Custom_Headers/>
      <Listserve_Options/>
      <external_ID/>
      <From_Email/>
      <From_Name/>
      <Reply_To/>
      <Headers_To_Remove/>
      <Confirmation_Message/>
      <Auto_Update_BOOLVALUE>false</Auto_Update_BOOLVALUE>
      <Auto_Update>false</Auto_Update>
      <query_KEY/>
      <Smart_Group_Options/>
      <Smart_Group_Error/>
      <enable_statistics_BOOLVALUE>false</enable_statistics_BOOLVALUE>
      <enable_statistics>false</enable_statistics>
      <join_email_trigger_KEYS/>
      <key>3</key>
      <object>groups</object>
    </item>
    <count>3</count>
  </groups>
</data>
EOF;
    }
    elseif ($query[0] == 'object=supporter_groups' && isset($query[1]) && $query[1] == 'condition=supporter_KEY=1') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <supporter_groups>
    <item>
      <supporter_groups_KEY>1</supporter_groups_KEY>
      <organization_KEY>11111</organization_KEY>
      <supporter_KEY>1</supporter_KEY>
      <groups_KEY>2</groups_KEY>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <Properties/>
      <key>1</key>
      <object>supporter_groups</object>
    </item>
    <count>1</count>
  </supporter_groups>
</data>
EOF;
    }
    elseif ($query[0] == 'object=tag') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <tag>
    <item>
      <tag_KEY>1</tag_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY>0</chapter_KEY>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <Date_Created>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Date_Created>
      <prefix/>
      <tag>Tag 1</tag>
      <label/>
      <Money_Spent/>
      <key>1</key>
      <object>tag</object>
    </item>
    <item>
      <tag_KEY>2</tag_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY>0</chapter_KEY>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <Date_Created>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Date_Created>
      <prefix/>
      <tag>Tag 2</tag>
      <label/>
      <Money_Spent/>
      <key>2</key>
      <object>tag</object>
    </item>
    <item>
      <tag_KEY>3</tag_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY>0</chapter_KEY>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <Date_Created>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Date_Created>
      <prefix/>
      <tag>Tag 3</tag>
      <label/>
      <Money_Spent/>
      <key>3</key>
      <object>tag</object>
    </item>
    <count>3</count>
  </tag>
</data>
EOF;
    }
    elseif ($query[0] == 'object=tag_data') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <tag_data>
    <item>
      <tag_data_KEY>1</tag_data_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY>0</chapter_KEY>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <database_table_KEY>142</database_table_KEY>
      <table_key>999</table_key>
      <tag_KEY>1</tag_KEY>
      <key>1</key>
      <object>tag_data</object>
    </item>
    <item>
      <tag_data_KEY>2</tag_data_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY>0</chapter_KEY>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <database_table_KEY>142</database_table_KEY>
      <table_key>999</table_key>
      <tag_KEY>2</tag_KEY>
      <key>2</key>
      <object>tag_data</object>
    </item>
    <count>2</count>
  </tag_data>
</data>
EOF;
    }
    elseif ($script == '/api/getObjects.sjs' && $query[0] == 'object=donate_page') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <donate_page>
    <item>
      <donate_page_KEY>1</donate_page_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY>0</chapter_KEY>
      <Reference_Name>Example Donate Page</Reference_Name>
      <Last_Modified>Tue May 15 2012 06:12:06 GMT-0400 (EDT)</Last_Modified>
      <Date_Created>Tue Feb 07 2012 10:39:24 GMT-0500 (EST)</Date_Created>
      <Header>Header Text</Header>
      <Request>__v2__,First_Name,Last_Name,Street,City,State,Zip,Email,Street_2</Request>
      <Required>Email</Required>
      <Allow_Anonymous_Donations_BOOLVALUE>false</Allow_Anonymous_Donations_BOOLVALUE>
      <Allow_Anonymous_Donations>false</Allow_Anonymous_Donations>
      <Pre_Custom_Text>Pre custom</Pre_Custom_Text>
      <PRIVATE_Custom_Fields/>
      <PRIVATE_Required_Custom_Fields/>
      <Tax_Status>Unknown</Tax_Status>
      <Pre_Donation_Text>&lt;p&gt;&lt;label for="Pre_Donation_Text"&gt;Pre Donation Text&lt;/label&gt; 	&lt;span id="Pre_Donation_Text.helpText" class="helpText"&gt;- Text you want to appear before the donation amounts&lt;/span&gt;&lt;/p&gt;</Pre_Donation_Text>
      <Comma_delimited_Amounts>50,100,300</Comma_delimited_Amounts>
      <Donation_Minimum>5.00</Donation_Minimum>
      <Donation_Limit>1000.00</Donation_Limit>
      <Confirm_Box_Text>Confirm Box Text - Enter the text you want to appear before the confirm donation check box</Confirm_Box_Text>
      <PreSubmit_Footer>Pre Submit Footer - Enter text that you want to appear right before the Submit button</PreSubmit_Footer>
      <Footer>Footer</Footer>
      <Thank_You_Page_Text>&lt;p&gt;&lt;label for="Thank_You_Page_Text"&gt;Thank you Page Text&lt;/label&gt;&lt;/p&gt;</Thank_You_Page_Text>
      <Number_of_columns>Two columns</Number_of_columns>
      <Default_Tracking_Code/>
      <redirect_path/>
      <PRIVATE_trigger/>
      <email_trigger_KEYS>189</email_trigger_KEYS>
      <merchant_account_KEY>55</merchant_account_KEY>
      <event_KEY>0</event_KEY>
      <groups_KEYS>0,37600,37601,37602,</groups_KEYS>
      <Include_Recurring_Payments_BOOLVALUE>true</Include_Recurring_Payments_BOOLVALUE>
      <Include_Recurring_Payments>true</Include_Recurring_Payments>
      <Include_Only_Recurring_Payments_BOOLVALUE>false</Include_Only_Recurring_Payments_BOOLVALUE>
      <Include_Only_Recurring_Payments>false</Include_Only_Recurring_Payments>
      <Include_In_Honor_Of_BOOLVALUE>false</Include_In_Honor_Of_BOOLVALUE>
      <Include_In_Honor_Of>false</Include_In_Honor_Of>
      <Include_Designation_Code_BOOLVALUE>false</Include_Designation_Code_BOOLVALUE>
      <Include_Designation_Code>false</Include_Designation_Code>
      <Include_Pay_By_Check_BOOLVALUE>false</Include_Pay_By_Check_BOOLVALUE>
      <Include_Pay_By_Check>false</Include_Pay_By_Check>
      <READONLY_Hit_Count>0</READONLY_Hit_Count>
      <Custom_Donation_HTML/>
      <donation_amount_KEYS/>
      <Template/>
      <Include_In_Memory_Of_BOOLVALUE>false</Include_In_Memory_Of_BOOLVALUE>
      <Include_In_Memory_Of>false</Include_In_Memory_Of>
      <Single_Click_Prefix/>
      <Include_Soft_Credit_BOOLVALUE>false</Include_Soft_Credit_BOOLVALUE>
      <Include_Soft_Credit>false</Include_Soft_Credit>
      <Disabled_BOOLVALUE>false</Disabled_BOOLVALUE>
      <Disabled>false</Disabled>
      <Goal>0.00</Goal>
      <linkRequest/>
      <linkRequired/>
      <membership_BOOLVALUE>false</membership_BOOLVALUE>
      <membership>false</membership>
      <key>1</key>
      <object>donate_page</object>
    </item>
    <count>1</count>
  </donate_page>
</data>
EOF;
    }
    elseif ($script == '/api/getObjects.sjs' && $query[0] == 'object=my_donate_page') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="17631">
<my_donate_page>
<item>
<my_donate_page_KEY>15</my_donate_page_KEY>
<organization_KEY>17631</organization_KEY>
<Title>Campaign 02</Title>
<Description>Campaign description...</Description>
<email_trigger_KEYS>0,190,209,202</email_trigger_KEYS>
<groups_KEYS>0,37600,37601,37602</groups_KEYS>
<Template>11870</Template>
<Search_Description>Customize the content on the search page</Search_Description>
<Create_Description>Creation page description...</Create_Description>
<Creation_Redirect/>
<Donation_Redirect/>
<Tell_a_Friend_Redirect/>
<Suggested_Title>Suggested title</Suggested_Title>
<Suggested_Description>Suggested donation page content...</Suggested_Description>
<Comma_delimited_Amounts>50, 100, 150, 200, 250</Comma_delimited_Amounts>
<Pre_Donation_Text>Pre donation text...</Pre_Donation_Text>
<Include_Recurring_Payments_BOOLVALUE>true</Include_Recurring_Payments_BOOLVALUE>
<Include_Recurring_Payments>true</Include_Recurring_Payments>
<merchant_account_KEY>55</merchant_account_KEY>
<Tax_Status>Unknown</Tax_Status>
<Donation_Page_Footer>Donation page footer...</Donation_Page_Footer>
<Suggested_Tell_a_Friend_Message>Suggested tell-a-friend message...</Suggested_Tell_a_Friend_Message>
<donor_groups_KEYS>0,37600,37601,37602</donor_groups_KEYS>
<donor/>
<Min_Donation_Goal>50</Min_Donation_Goal>
<Max_Donation_Goal>2500</Max_Donation_Goal>
<Request>First_Name,Last_Name,Street,City,State,Zip,Email,hide_last_name
</Request>
<Required>,Email</Required>
<Tell_a_Friend_Subject>Tell-a-Friend Subject</Tell_a_Friend_Subject>
<Donation_Limit/>
<Confirm_p2p_BOOLVALUE>false</Confirm_p2p_BOOLVALUE>
<Confirm_p2p>false</Confirm_p2p>
<key>15</key>
<object>my_donate_page</object>
<event_id>10</event_id>
<event_sub_id>15</event_sub_id>
</item>
<count>1</count>
</my_donate_page>
</data>
EOF;
    }
    elseif ($query[0] == 'object=database_table' && isset($query[1]) && $query[1] == 'condition=table_name=supporter') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <database_table>
    <item>
      <database_table_KEY>142</database_table_KEY>
      <package_KEY>0</package_KEY>
      <organization_KEY>0</organization_KEY>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <Date_Created>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Date_Created>
      <table_name>supporter</table_name>
      <label/>
      <create_permission/>
      <update_permission>Campaign Managers</update_permission>
      <is_virtual_BOOLVALUE>false</is_virtual_BOOLVALUE>
      <is_virtual>false</is_virtual>
      <actual_data_table/>
      <campaign_manager_permission>SELECT,INSERT,UPDATE,DELETE</campaign_manager_permission>
      <supporter_permission>SELECT,INSERT,UPDATE</supporter_permission>
      <public_permission>SELECT,INSERT</public_permission>
      <key>142</key>
      <object>database_table</object>
    </item>
    <count>1</count>
  </database_table>
</data>
EOF;
    }
    elseif ($script == '/api/getObjects.sjs' && $query[0] == 'object=custom_column') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <custom_column>
    <item>
      <custom_column_KEY>1</custom_column_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY/>
      <database_table_KEY>142</database_table_KEY>
      <data_table>supporter_custom</data_table>
      <data_column>VARCHAR0</data_column>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <Date_Created>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Date_Created>
      <name>uid</name>
      <label>UID</label>
      <type>varchar</type>
      <Display_Name/>
      <Description/>
      <_Order>0</_Order>
      <Display_To_Supporters_BOOLVALUE>false</Display_To_Supporters_BOOLVALUE>
      <Display_To_Supporters>false</Display_To_Supporters>
      <old_custom_field_KEY/>
      <expose_to_chapters_BOOLVALUE>false</expose_to_chapters_BOOLVALUE>
      <expose_to_chapters>false</expose_to_chapters>
      <Is_Reserved_BOOLVALUE>false</Is_Reserved_BOOLVALUE>
      <Is_Reserved>false</Is_Reserved>
      <is_a_zero_index_enum_BOOLVALUE>false</is_a_zero_index_enum_BOOLVALUE>
      <is_a_zero_index_enum>false</is_a_zero_index_enum>
      <key>1</key>
      <object>custom_column</object>
    </item>
    <item>
      <custom_column_KEY>2</custom_column_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY/>
      <database_table_KEY>142</database_table_KEY>
      <data_table>supporter_custom</data_table>
      <data_column>VARCHAR1</data_column>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <Date_Created>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Date_Created>
      <name>custom_field_1</name>
      <label>Custom field 1</label>
      <type>varchar</type>
      <Display_Name/>
      <Description/>
      <_Order>0</_Order>
      <Display_To_Supporters_BOOLVALUE>false</Display_To_Supporters_BOOLVALUE>
      <Display_To_Supporters>false</Display_To_Supporters>
      <old_custom_field_KEY/>
      <expose_to_chapters_BOOLVALUE>false</expose_to_chapters_BOOLVALUE>
      <expose_to_chapters>false</expose_to_chapters>
      <Is_Reserved_BOOLVALUE>false</Is_Reserved_BOOLVALUE>
      <Is_Reserved>false</Is_Reserved>
      <is_a_zero_index_enum_BOOLVALUE>false</is_a_zero_index_enum_BOOLVALUE>
      <is_a_zero_index_enum>false</is_a_zero_index_enum>
      <key>2</key>
      <object>custom_column</object>
    </item>
    <item>
      <custom_column_KEY>3</custom_column_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY/>
      <database_table_KEY>142</database_table_KEY>
      <data_table>supporter_custom</data_table>
      <data_column>VARCHAR1</data_column>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <Date_Created>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Date_Created>
      <name>custom_field_2</name>
      <label>Custom field 2</label>
      <type>varchar</type>
      <Display_Name/>
      <Description/>
      <_Order>0</_Order>
      <Display_To_Supporters_BOOLVALUE>false</Display_To_Supporters_BOOLVALUE>
      <Display_To_Supporters>false</Display_To_Supporters>
      <old_custom_field_KEY/>
      <expose_to_chapters_BOOLVALUE>false</expose_to_chapters_BOOLVALUE>
      <expose_to_chapters>false</expose_to_chapters>
      <Is_Reserved_BOOLVALUE>false</Is_Reserved_BOOLVALUE>
      <Is_Reserved>false</Is_Reserved>
      <is_a_zero_index_enum_BOOLVALUE>false</is_a_zero_index_enum_BOOLVALUE>
      <is_a_zero_index_enum>false</is_a_zero_index_enum>
      <key>3</key>
      <object>custom_column</object>
    </item>
    <count>3</count>
  </custom_column>
</data>
EOF;
    }
    elseif ($query[0] == 'object=unsubscribe_page') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <unsubscribe_page>
    <item>
      <unsubscribe_page_KEY>1</unsubscribe_page_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY/>
      <Reference_Name>Unsubscribe Test Page</Reference_Name>
      <Last_Modified>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Last_Modified>
      <Date_Created>Fri Aug 31 2012 00:00:00 GMT-0400 (EDT)</Date_Created>
      <Title>Unsubscribe Test Page</Title>
      <Header>Unsubscribe page header</Header>
      <Footer>Unsubscribe page footer</Footer>
      <PreGroup_Text/>
      <groups_KEYS>1,2,3</groups_KEYS>
      <PreTag_Text/>
      <tag_KEYS>1,2,3</tag_KEYS>
      <redirect_path/>
      <email_trigger_KEYS/>
      <hide_remove_from_emaillist_BOOLVALUE>false</hide_remove_from_emaillist_BOOLVALUE>
      <hide_remove_from_emaillist>false</hide_remove_from_emaillist>
      <template/>
      <PreChapter_Text/>
      <chapter_KEYS/>
      <show_reason_field_BOOLVALUE>true</show_reason_field_BOOLVALUE>
      <show_reason_field>true</show_reason_field>
      <key>1</key>
      <object>unsubscribe_page</object>
    </item>
    <count>1</count>
  </unsubscribe_page>
</data>
EOF;
    }

    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=supporter_my_donate_page_donation') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="50444">
  <supporter_my_donate_page_donation>
    <item>
      <name>supporter_my_donate_page_donation_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter my donate page donation KEY</label>
    </item>
    <item>
      <name>supporter_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter KEY</label>
    </item>
    <item>
      <name>donation_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>donation KEY</label>
    </item>
    <item>
      <name>recurring_donation_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>recurring donation KEY</label>
    </item>
    <item>
      <name>supporter_my_donate_page_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter my donate page KEY</label>
    </item>
  </supporter_my_donate_page_donation>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=supporter_my_donate_page') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="0">
  <supporter_my_donate_page>
    <item>
      <name>supporter_my_donate_page_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter my donate page KEY</label>
    </item>
    <item>
      <name>supporter_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter KEY</label>
    </item>
    <item>
      <name>my_donate_page_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>my donate page KEY</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <name>Date_Created</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Date Created</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Page_Title</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Page Title</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Description</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Description</label>
    </item>
    <item>
      <name>Donation_Goal</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>Donation Goal</label>
    </item>
    <item>
      <values>Public,Private,Unconfirmed</values>
      <name>Status</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Status</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Image</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Image</label>
    </item>
  </supporter_my_donate_page>
</data>
EOF;
    }

    if ($script == '/api/describe2.sjs' && $query[0] == 'object=donation') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="0">
  <donation>
    <item>
      <name>donation_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>donation KEY</label>
    </item>
    <item>
      <name>supporter_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter KEY</label>
    </item>
    <item>
      <name>event_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>event KEY</label>
    </item>
    <item>
      <name>donate_page_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>donate page KEY</label>
    </item>
    <item>
      <name>recurring_donation_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>recurring donation KEY</label>
    </item>
    <item>
      <name>membership_invoice_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>membership invoice KEY</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <name>Transaction_Date</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Transaction Date</label>
    </item>
    <item>
      <name>Date_Entered</name>
      <nullable>0</nullable>
      <type>date</type>
      <defaultValue/>
      <label>Date Entered</label>
    </item>
    <item>
      <name>amount</name>
      <nullable>0</nullable>
      <type>decimal</type>
      <defaultValue/>
      <label>amount</label>
    </item>
    <item>
      <maxlength>3</maxlength>
      <name>Currency_Code</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Currency Code</label>
    </item>
    <item>
      <values>Donation,Purchase,Payment,Recurring,Other,New Member,Renewal,Gift Membership,Event Fee,Publication Purchase,Testing,Refund,In-Kind</values>
      <name>Transaction_Type</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Transaction Type</label>
    </item>
    <item>
      <values>Credit Card,Check,Cash,Other,Unknown</values>
      <name>Form_Of_Payment</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Form Of Payment</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>cc_type</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>cc type</label>
    </item>
    <item>
      <maxlength>16</maxlength>
      <name>Credit_Card_Digits</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Credit Card Digits</label>
    </item>
    <item>
      <maxlength>5</maxlength>
      <name>Credit_Card_Expiration</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Credit Card Expiration</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>PNREF</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>PNREF</label>
    </item>
    <item>
      <name>RESULT</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>RESULT</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>RESPMSG</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>RESPMSG</label>
    </item>
    <item>
      <maxlength>8</maxlength>
      <name>AUTHCODE</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>AUTHCODE</label>
    </item>
    <item>
      <maxlength>10</maxlength>
      <name>AVS</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>AVS</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Order_Info</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Order Info</label>
    </item>
    <item>
      <values>Donation Only,New Order,Order Pending,Order Fulfilled,Transaction Failed,Refunded,Event Fee</values>
      <name>Status</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Status</label>
    </item>
    <item>
      <values>Unknown,In Account,Disbursed,Refunded,Other</values>
      <name>Disbursement_Status</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue>Unknown</defaultValue>
      <label>Disbursement Status</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>Responsible_Party</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Responsible Party</label>
    </item>
    <item>
      <name>Date_Fulfilled</name>
      <nullable>0</nullable>
      <type>date</type>
      <defaultValue/>
      <label>Date Fulfilled</label>
    </item>
    <item>
      <name>PRIVATE_Complete_Summary</name>
      <nullable>0</nullable>
      <type>blob</type>
      <defaultValue/>
      <label>PRIVATE Complete Summary</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>First_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>First Name</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Last_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Last Name</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>Email</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Email</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Tracking_Code</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Tracking Code</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Donation_Tracking_Code</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Donation Tracking Code</label>
    </item>
    <item>
      <values>Unknown,C3,C4,527,PAC</values>
      <name>Tax_Status</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Tax Status</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Designation_Code</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Designation Code</label>
    </item>
    <item>
      <values>DIA,Other</values>
      <name>PRIVATE_Donation_Source</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue>Other</defaultValue>
      <label>PRIVATE Donation Source</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Note</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Note</label>
    </item>
    <item>
      <name>Thank_You_Sent</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue/>
      <label>Thank You Sent</label>
    </item>
    <item>
      <name>Thank_Date</name>
      <nullable>0</nullable>
      <type>date</type>
      <defaultValue/>
      <label>Thank Date</label>
    </item>
    <item>
      <name>referral_supporter_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>referral supporter KEY</label>
    </item>
    <item>
      <name>merchant_account_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>merchant account KEY</label>
    </item>
    <item>
      <maxlength>16</maxlength>
      <name>IP_Address</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>IP Address</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>In_Honor_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>In Honor Name</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>In_Honor_Email</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>In Honor Email</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>In_Honor_Address</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>In Honor Address</label>
    </item>
    <item>
      <maxlength>16</maxlength>
      <name>uid</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>uid</label>
    </item>
    <item>
      <maxlength>16</maxlength>
      <name>Batch_Code</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Batch Code</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>VARCHAR0</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>VARCHAR0</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>VARCHAR1</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>VARCHAR1</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>VARCHAR2</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>VARCHAR2</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>In_Memory_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>In Memory Name</label>
    </item>
    <item>
      <name>event_fee_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>event fee KEY</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Employer</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Employer</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Occupation</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Occupation</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Employer_Street</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Employer Street</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Employer_Street_2</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Employer Street 2</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>Employer_City</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Employer City</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>Employer_State</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Employer State</label>
    </item>
    <item>
      <maxlength>10</maxlength>
      <name>Employer_Zip</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Employer Zip</label>
    </item>
  </donation>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=recurring_donation') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="0">
  <recurring_donation>
    <item>
      <name>recurring_donation_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>recurring donation KEY</label>
    </item>
    <item>
      <name>supporter_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter KEY</label>
    </item>
    <item>
      <name>donation_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>donation KEY</label>
    </item>
    <item>
      <name>merchant_account_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>merchant account KEY</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <name>Transaction_Date</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Transaction Date</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>RPREF</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>RPREF</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>TRXPNREF</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>TRXPNREF</label>
    </item>
    <item>
      <name>RESULT</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>RESULT</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>PROFILEID</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>PROFILEID</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>RESPMSG</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>RESPMSG</label>
    </item>
    <item>
      <name>amount</name>
      <nullable>0</nullable>
      <type>float</type>
      <defaultValue/>
      <label>amount</label>
    </item>
    <item>
      <name>Start_Date</name>
      <nullable>0</nullable>
      <type>date</type>
      <defaultValue/>
      <label>Start Date</label>
    </item>
    <item>
      <maxlength>8</maxlength>
      <name>PAYPERIOD</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>PAYPERIOD</label>
    </item>
    <item>
      <name>TERM</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>TERM</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Tracking_Code</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Tracking Code</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Designation_Code</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Designation Code</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Email</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Email</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>First_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>First Name</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Last_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Last Name</label>
    </item>
    <item>
      <values>Unknown,C3,C4,527,PAC</values>
      <name>Tax_Status</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Tax Status</label>
    </item>
    <item>
      <values>,Active,Inactive,Failed,Paused</values>
      <name>Status</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue>Active</defaultValue>
      <label>Status</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Note</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Note</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Error_Message</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Error Message</label>
    </item>
    <item>
      <name>source_donation_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>source donation KEY</label>
    </item>
    <item>
      <name>current_reference_donation_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>current reference donation KEY</label>
    </item>
    <item>
      <name>run_one_transaction</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue>0</defaultValue>
      <label>run one transaction</label>
    </item>
    <item>
      <name>donate_page_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>donate page KEY</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>cc_type</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>cc type</label>
    </item>
    <item>
      <maxlength>5</maxlength>
      <name>Credit_Card_Expiration</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Credit Card Expiration</label>
    </item>
  </recurring_donation>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=supporter') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="0">
  <supporter>
    <item>
      <name>supporter_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter KEY</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <name>Date_Created</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Date Created</label>
    </item>
    <item>
      <maxlength>16</maxlength>
      <name>Title</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Title</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>First_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>First Name</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>MI</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>MI</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Last_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Last Name</label>
    </item>
    <item>
      <maxlength>16</maxlength>
      <name>Suffix</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Suffix</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Email</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Email</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>Password</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Password</label>
    </item>
    <item>
      <name>Receive_Email</name>
      <nullable>1</nullable>
      <type>tinyint</type>
      <defaultValue>1</defaultValue>
      <label>Receive Email</label>
    </item>
    <item>
      <values>N/A,Good,Soft Bounce,Hard Bounce</values>
      <name>Email_Status</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Email Status</label>
    </item>
    <item>
      <values>HTML Email,Text Email</values>
      <name>Email_Preference</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue>HTML Email</defaultValue>
      <label>Email Preference</label>
    </item>
    <item>
      <name>Soft_Bounce_Count</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>Soft Bounce Count</label>
    </item>
    <item>
      <name>Hard_Bounce_Count</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>Hard Bounce Count</label>
    </item>
    <item>
      <name>Last_Bounce</name>
      <nullable>0</nullable>
      <type>datetime</type>
      <defaultValue/>
      <label>Last Bounce</label>
    </item>
    <item>
      <name>Receive_Phone_Blasts</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue/>
      <label>Receive Phone Blasts</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Phone</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Phone</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Cell_Phone</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Cell Phone</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Phone_Provider</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Phone Provider</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Work_Phone</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Work Phone</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Pager</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Pager</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Home_Fax</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Home Fax</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Work_Fax</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Work Fax</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Street</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Street</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Street_2</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Street 2</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Street_3</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Street 3</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>City</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>City</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>State</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>State</label>
    </item>
    <item>
      <maxlength>10</maxlength>
      <name>Zip</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Zip</label>
    </item>
    <item>
      <maxlength>4</maxlength>
      <name>PRIVATE_Zip_Plus_4</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>PRIVATE Zip Plus 4</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>County</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>County</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>District</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>District</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Country</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Country</label>
    </item>
    <item>
      <name>Latitude</name>
      <nullable>0</nullable>
      <type>float</type>
      <defaultValue/>
      <label>Latitude</label>
    </item>
    <item>
      <name>Longitude</name>
      <nullable>0</nullable>
      <type>float</type>
      <defaultValue/>
      <label>Longitude</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Organization</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Organization</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Department</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Department</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Occupation</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Occupation</label>
    </item>
    <item>
      <values>AOL,Yahoo,MSN,ICQ,Other</values>
      <name>Instant_Messenger_Service</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Instant Messenger Service</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Instant_Messenger_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Instant Messenger Name</label>
    </item>
    <item>
      <maxlength>127</maxlength>
      <name>Web_Page</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Web Page</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Alternative_Email</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Alternative Email</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>Other_Data_1</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Other Data 1</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Other_Data_2</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Other Data 2</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Other_Data_3</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Other Data 3</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Notes</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Notes</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Source</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Source</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Source_Details</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Source Details</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Source_Tracking_Code</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Source Tracking Code</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Tracking_Code</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Tracking Code</label>
    </item>
    <item>
      <values>Active,Temporarily Inactive,Invisible,Inactive</values>
      <name>Status</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Status</label>
    </item>
    <item>
      <maxlength>16</maxlength>
      <name>uid</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>UID</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>custom_field_1</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Custom Field 1</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>custom_field_2</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>custom_field_2</label>
    </item>
    <item>
      <maxlength>16</maxlength>
      <name>Timezone</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Timezone</label>
    </item>
    <item>
      <maxlength>3</maxlength>
      <name>Language_Code</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Language Code</label>
    </item>
  </supporter>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=signup_page') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <signup_page>
    <item>
      <name>signup_page_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>signup page KEY</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Reference_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Reference Name</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <name>Date_Created</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Date Created</label>
    </item>
    <item>
      <values>Normal,Simple Survey,Contact Page</values>
      <name>Style</name>
      <nullable>1</nullable>
      <type>enum</type>
      <defaultValue>Normal</defaultValue>
      <label>Style</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Title</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Title</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Header</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Header</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Request</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Request</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Required</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Required</label>
    </item>
    <item>
      <name>Include_Remember_Me_Link</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue/>
      <label>Include Remember Me Link</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Opt_in_Text</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Opt in Text</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>PreFooter</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>PreFooter</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Footer</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Footer</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>PreGroup_Text</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>PreGroup Text</label>
    </item>
    <item>
      <name>groups_KEYS</name>
      <nullable>0</nullable>
      <type>blob</type>
      <defaultValue/>
      <label>groups KEYS</label>
    </item>
    <item>
      <name>Automatically_add_to_Groups</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue/>
      <label>Automatically add to Groups</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>PreInterest_Text</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>PreInterest Text</label>
    </item>
    <item>
      <name>interest_KEYS</name>
      <nullable>0</nullable>
      <type>blob</type>
      <defaultValue/>
      <label>interest KEYS</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>tag_KEYS</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>tag KEYS</label>
    </item>
    <item>
      <name>event_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>event KEY</label>
    </item>
    <item>
      <name>Request_Volunteer_Availability</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue/>
      <label>Request Volunteer Availability</label>
    </item>
    <item>
      <name>Email_Confirmed_Opt_In</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue/>
      <label>Email Confirmed Opt In</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Default_Tracking_Code</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Default Tracking Code</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>redirect_path</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>redirect path</label>
    </item>
    <item>
      <name>Maximum_Field_Width</name>
      <nullable>0</nullable>
      <type>smallint</type>
      <defaultValue/>
      <label>Maximum Field Width</label>
    </item>
    <item>
      <values>,On Signup,Custom A,Custom B,Custom C,Custom D,Custom E,Custom F,Custom G</values>
      <name>PRIVATE_trigger</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>PRIVATE trigger</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>email_trigger_KEYS</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>email trigger KEYS</label>
    </item>
    <item>
      <name>READONLY_Hit_Count</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>READONLY Hit Count</label>
    </item>
    <item>
      <values>,State,Zip</values>
      <name>chapter_lookup</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>chapter lookup</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Template</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Template</label>
    </item>
    <item>
      <name>add_to_groups_KEYS</name>
      <nullable>0</nullable>
      <type>mediumtext</type>
      <defaultValue/>
      <label>add to groups KEYS</label>
    </item>
    <item>
      <name>optionally_add_to_groups_KEYS</name>
      <nullable>0</nullable>
      <type>mediumtext</type>
      <defaultValue/>
      <label>optionally add to groups KEYS</label>
    </item>
  </signup_page>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=groups') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <groups>
    <item>
      <name>groups_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>groups KEY</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <name>Date_Created</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Date Created</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Group_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Group Name</label>
    </item>
    <item>
      <maxlength>240</maxlength>
      <name>Reference_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Reference Name</label>
    </item>
    <item>
      <name>parent_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>parent KEY</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>Description</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Description</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Notes</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Notes</label>
    </item>
    <item>
      <name>Display_To_User</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue/>
      <label>Display To User</label>
    </item>
    <item>
      <values>No Listserve,Open Discussion List,Moderated List,Restrict Posts to Allowed Users</values>
      <name>Listserve_Type</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Listserve Type</label>
    </item>
    <item>
      <values>Open,Closed,Approved Only</values>
      <name>Subscription_Type</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Subscription Type</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Manager</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Manager</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Moderator_Emails</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Moderator Emails</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>Subject_Prefix</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Subject Prefix</label>
    </item>
    <item>
      <values>From Sender,Reply-To Sender,Anonymous</values>
      <name>Listserve_Responses</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Listserve Responses</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Append_Header</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Append Header</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Append_Footer</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Append Footer</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Custom_Headers</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Custom Headers</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Listserve_Options</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Listserve Options</label>
    </item>
    <item>
      <maxlength>8</maxlength>
      <name>external_ID</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>external ID</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>From_Email</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>From Email</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>From_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>From Name</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Reply_To</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Reply To</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>Headers_To_Remove</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Headers To Remove</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Confirmation_Message</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Confirmation Message</label>
    </item>
    <item>
      <name>Auto_Update</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue>0</defaultValue>
      <label>Auto Update</label>
    </item>
    <item>
      <name>query_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>query KEY</label>
    </item>
    <item>
      <values>Off,Smart Group,Greedy Smart Group</values>
      <name>Smart_Group_Options</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue>Off</defaultValue>
      <label>Smart Group Options</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>Smart_Group_Error</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Smart Group Error</label>
    </item>
    <item>
      <name>enable_statistics</name>
      <nullable>1</nullable>
      <type>tinyint</type>
      <defaultValue>0</defaultValue>
      <label>enable statistics</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>join_email_trigger_KEYS</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>join email trigger KEYS</label>
    </item>
  </groups>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=tag') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <tag>
    <item>
      <name>tag_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>tag KEY</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <name>Date_Created</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Date Created</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>prefix</name>
      <nullable>1</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>prefix</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>tag</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>tag</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>label</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>label</label>
    </item>
    <item>
      <name>Money_Spent</name>
      <nullable>0</nullable>
      <type>float</type>
      <defaultValue/>
      <label>Money Spent</label>
    </item>
  </tag>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=tag_data') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <tag_data>
    <item>
      <name>tag_data_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>tag data KEY</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <name>database_table_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>database table KEY</label>
    </item>
    <item>
      <name>table_key</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>table key</label>
    </item>
    <item>
      <name>tag_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>tag KEY</label>
    </item>
  </tag_data>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=supporter_groups') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <supporter_groups>
    <item>
      <name>supporter_groups_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter groups KEY</label>
    </item>
    <item>
      <name>supporter_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue>0</defaultValue>
      <label>supporter KEY</label>
    </item>
    <item>
      <name>groups_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue>0</defaultValue>
      <label>groups KEY</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <values>Pending,Pending Unsubscribe,Allowed to send Email,Moderator,Unmoderated,Receive Daily Digests,Receive Weekly Digests,Receive No Emails</values>
      <name>Properties</name>
      <nullable>0</nullable>
      <type>set</type>
      <defaultValue/>
      <multiple>true</multiple>
      <label>Properties</label>
    </item>
  </supporter_groups>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=database_table') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <database_table>
    <item>
      <name>database_table_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>database table KEY</label>
    </item>
    <item>
      <name>package_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue>0</defaultValue>
      <label>package KEY</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <name>Date_Created</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Date Created</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>table_name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>table name</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>label</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>label</label>
    </item>
    <item>
      <values>Campaign Managers,Supporters,Public</values>
      <name>create_permission</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>create permission</label>
    </item>
    <item>
      <values>Campaign Managers,Supporters,Public</values>
      <name>update_permission</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>update permission</label>
    </item>
    <item>
      <name>is_virtual</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue>0</defaultValue>
      <label>is virtual</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>actual_data_table</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>actual data table</label>
    </item>
    <item>
      <values>SELECT,INSERT,UPDATE,DELETE</values>
      <name>campaign_manager_permission</name>
      <nullable>0</nullable>
      <type>set</type>
      <defaultValue>SELECT,INSERT,UPDATE,DELETE</defaultValue>
      <multiple>true</multiple>
      <label>campaign manager permission</label>
    </item>
    <item>
      <values>SELECT,INSERT,UPDATE,DELETE</values>
      <name>supporter_permission</name>
      <nullable>0</nullable>
      <type>set</type>
      <defaultValue>SELECT</defaultValue>
      <multiple>true</multiple>
      <label>supporter permission</label>
    </item>
    <item>
      <values>SELECT,INSERT,UPDATE,DELETE</values>
      <name>public_permission</name>
      <nullable>0</nullable>
      <type>set</type>
      <defaultValue>SELECT</defaultValue>
      <multiple>true</multiple>
      <label>public permission</label>
    </item>
  </database_table>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=custom_column') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <custom_column>
    <item>
      <name>custom_column_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>custom column KEY</label>
    </item>
    <item>
      <name>database_table_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>database table KEY</label>
    </item>
    <item>
      <values>supporter_custom,supporter_custom2,supporter_custom3,supporter_custom4,supporter_custom5,supporter_custom6,supporter_custom7,supporter_custom8,supporter_custom9,supporter_custom10,supporter_custom11,supporter_custom12,supporter_custom13,supporter_custom14,supporter_custom15,supporter_custom16,supporter_custom17,supporter_custom18,supporter_custom19,supporter_custom20,supporter_custom21,supporter_custom22,supporter_custom23,supporter_custom24,supporter_custom25,custom_data_int,custom_data_int2,custom_data_int3,custom_data_int4,custom_data_int5,custom_data_int6,custom_data_int7,custom_data_int8,custom_data_int9,custom_data_int10,custom_data_int11,custom_data_int12,custom_data_int13,custom_data_int14,custom_data_int15,custom_data_int16,custom_data_int17,custom_data_int18,custom_data_int19,custom_data_int20,custom_data_int21,custom_data_int22,custom_data_int23,custom_data_int24,custom_data_int25,custom_data_bool,custom_data_bool2,custom_data_bool3,custom_data_bool4,custom_data_bool5,custom_data_bool6,custom_data_bool7,custom_data_bool8,custom_data_bool9,custom_data_bool10,custom_data_bool11,custom_data_bool12,custom_data_bool13,custom_data_bool14,custom_data_bool15,custom_data_bool16,custom_data_bool17,custom_data_bool18,custom_data_bool19,custom_data_bool20,custom_data_bool21,custom_data_bool22,custom_data_bool23,custom_data_bool24,custom_data_bool25,custom_data_enum,custom_data_enum2,custom_data_enum3,custom_data_enum4,custom_data_enum5,custom_data_enum6,custom_data_enum7,custom_data_enum8,custom_data_enum9,custom_data_enum10,custom_data_enum11,custom_data_enum12,custom_data_enum13,custom_data_enum14,custom_data_enum15,custom_data_enum16,custom_data_enum17,custom_data_enum18,custom_data_enum19,custom_data_enum20,custom_data_enum21,custom_data_enum22,custom_data_enum23,custom_data_enum24,custom_data_enum25,custom_data_timestamp,custom_data_timestamp2,custom_data_timestamp3,custom_data_timestamp4,custom_data_timestamp5,custom_data_timestamp6,custom_data_timestamp7,custom_data_timestamp8,custom_data_timestamp9,custom_data_timestamp10,custom_data_timestamp11,custom_data_timestamp12,custom_data_timestamp13,custom_data_timestamp14,custom_data_timestamp15,custom_data_timestamp16,custom_data_timestamp17,custom_data_timestamp18,custom_data_timestamp19,custom_data_timestamp20,custom_data_timestamp21,custom_data_timestamp22,custom_data_timestamp23,custom_data_timestamp24,custom_data_timestamp25,custom_data_text,custom_data_text2,custom_data_text3,custom_data_text4,custom_data_text5,custom_data_text6,custom_data_text7,custom_data_text8,custom_data_text9,custom_data_text10,custom_data_text11,custom_data_text12,custom_data_text13,custom_data_text14,custom_data_text15,custom_data_text16,custom_data_text17,custom_data_text18,custom_data_text19,custom_data_text20,custom_data_text21,custom_data_text22,custom_data_text23,custom_data_text24,custom_data_text25,custom_data_varchar,custom_data_varchar2,custom_data_varchar3,custom_data_varchar4,custom_data_varchar5,custom_data_varchar6,custom_data_varchar7,custom_data_varchar8,custom_data_varchar9,custom_data_varchar10,custom_data_varchar11,custom_data_varchar12,custom_data_varchar13,custom_data_varchar14,custom_data_varchar15,custom_data_varchar16,custom_data_varchar17,custom_data_varchar18,custom_data_varchar19,custom_data_varchar20,custom_data_varchar21,custom_data_varchar22,custom_data_varchar23,custom_data_varchar24,custom_data_varchar25,`salesforce`.custom_data,`salesforce`.custom_data2,`salesforce`.custom_data3,`salesforce`.custom_data4,`salesforce`.custom_data5,`salesforce`.custom_data6,`salesforce`.custom_data7,`salesforce`.custom_data8,`salesforce`.custom_data9,`salesforce`.custom_data10,`salesforce`.custom_data11,`salesforce`.custom_data12,`salesforce`.custom_data13,`salesforce`.custom_data14,`salesforce`.custom_data15,`salesforce`.custom_data16,`salesforce`.custom_data17,`salesforce`.custom_data18,`salesforce`.custom_data19,`salesforce`.custom_data20,`salesforce`.custom_data21,`salesforce`.custom_data22,`salesforce`.custom_data23,`salesforce`.custom_data24,`salesforce`.custom_data25,custom_data_percent,custom_data_percent2,custom_data_percent3,custom_data_percent4,custom_data_percent5,custom_data_percent6,custom_data_percent7,custom_data_percent8,custom_data_percent9,custom_data_percent10,custom_data_percent11,custom_data_percent12,custom_data_percent13,custom_data_percent14,custom_data_percent15,custom_data_percent16,custom_data_percent17,custom_data_percent18,custom_data_percent19,custom_data_percent20,custom_data_percent21,custom_data_percent22,custom_data_percent23,custom_data_percent24,custom_data_percent25,custom_data_currency,custom_data_currency2,custom_data_currency3,custom_data_currency4,custom_data_currency5,custom_data_currency6,custom_data_currency7,custom_data_currency8,custom_data_currency9,custom_data_currency10,custom_data_currency11,custom_data_currency12,custom_data_currency13,custom_data_currency14,custom_data_currency15,custom_data_currency16,custom_data_currency17,custom_data_currency18,custom_data_currency19,custom_data_currency20,custom_data_currency21,custom_data_currency22,custom_data_currency23,custom_data_currency24,custom_data_currency25,custom_data_float,custom_data_float2,custom_data_float3,custom_data_float4,custom_data_float5,custom_data_float6,custom_data_float7,custom_data_float8,custom_data_float9,custom_data_float10,custom_data_float11,custom_data_float12,custom_data_float13,custom_data_float14,custom_data_float15,custom_data_float16,custom_data_float17,custom_data_float18,custom_data_float19,custom_data_float20,custom_data_float21,custom_data_float22,custom_data_float23,custom_data_float24,custom_data_float25,custom_data_datetime,custom_data_datetime2,custom_data_datetime3,custom_data_datetime4,custom_data_datetime5,custom_data_datetime6,custom_data_datetime7,custom_data_datetime8,custom_data_datetime9,custom_data_datetime10,custom_data_datetime11,custom_data_datetime12,custom_data_datetime13,custom_data_datetime14,custom_data_datetime15,custom_data_datetime16,custom_data_datetime17,custom_data_datetime18,custom_data_datetime19,custom_data_datetime20,custom_data_datetime21,custom_data_datetime22,custom_data_datetime23,custom_data_datetime24,custom_data_datetime25,custom_data_ga,custom_data_ga2,custom_data_ga3,custom_data_ga4,custom_data_ga5,custom_data_ga6,custom_data_ga7,custom_data_ga8,custom_data_ga9,custom_data_ga10,custom_data_ga11,custom_data_ga12,custom_data_ga13,custom_data_ga14,custom_data_ga15,custom_data_ga16,custom_data_ga17,custom_data_ga18,custom_data_ga19,custom_data_ga20,custom_data_ga21,custom_data_ga22,custom_data_ga23,custom_data_ga24,custom_data_ga25</values>
      <name>data_table</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>data table</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>data_column</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>data column</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <name>Date_Created</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Date Created</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>name</name>
      <nullable>1</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>name</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>label</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>label</label>
    </item>
    <item>
      <values>varchar,text,mediumtext,int,bool,float,date,time,datetime,timestamp,enum,percent,currency</values>
      <name>type</name>
      <nullable>1</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>type</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Display_Name</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Display Name</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Description</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Description</label>
    </item>
    <item>
      <name>_Order</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>Order</label>
    </item>
    <item>
      <name>Display_To_Supporters</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue/>
      <label>Display To Supporters</label>
    </item>
    <item>
      <name>old_custom_field_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>old custom field KEY</label>
    </item>
    <item>
      <name>expose_to_chapters</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue>0</defaultValue>
      <label>expose to chapters</label>
    </item>
    <item>
      <name>Is_Reserved</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue>0</defaultValue>
      <label>Is Reserved</label>
    </item>
    <item>
      <name>is_a_zero_index_enum</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue>0</defaultValue>
      <label>is a zero index enum</label>
    </item>
  </custom_column>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=unsubscribe_page') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <unsubscribe_page>
    <item>
      <name>unsubscribe_page_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>unsubscribe page KEY</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Reference_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Reference Name</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <name>Date_Created</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Date Created</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Title</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Title</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Header</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Header</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>Footer</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>Footer</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>PreGroup_Text</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>PreGroup Text</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>groups_KEYS</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>groups KEYS</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>PreTag_Text</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>PreTag Text</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>tag_KEYS</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>tag KEYS</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>redirect_path</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>redirect path</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>email_trigger_KEYS</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>email trigger KEYS</label>
    </item>
    <item>
      <name>hide_remove_from_emaillist</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue/>
      <label>hide remove from emaillist</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>template</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>template</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>PreChapter_Text</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>PreChapter Text</label>
    </item>
    <item>
      <maxlength>32767</maxlength>
      <name>chapter_KEYS</name>
      <nullable>0</nullable>
      <type>text</type>
      <defaultValue/>
      <label>chapter KEYS</label>
    </item>
    <item>
      <name>show_reason_field</name>
      <nullable>1</nullable>
      <type>tinyint</type>
      <defaultValue>0</defaultValue>
      <label>show reason field</label>
    </item>
  </unsubscribe_page>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=unsubscribe') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <unsubscribe>
    <item>
      <name>unsubscribe_key</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>unsubscribe key</label>
    </item>
    <item>
      <name>supporter_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter KEY</label>
    </item>
    <item>
      <name>email_blast_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>email blast KEY</label>
    </item>
    <item>
      <name>last_modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>last modified</label>
    </item>
    <item>
      <name>Unsubscribe_Date</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Unsubscribe Date</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>Email</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Email</label>
    </item>
    <item>
      <values>Web Page,Email,Soft Bounce Count,Hard Bounce Count,Email Filter,Other</values>
      <name>Unsubscribe_Method</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Unsubscribe Method</label>
    </item>
    <item>
      <maxlength>128</maxlength>
      <name>Details</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Details</label>
    </item>
    <item>
      <name>Supporter_Removed</name>
      <nullable>0</nullable>
      <type>tinyint</type>
      <defaultValue/>
      <label>Supporter Removed</label>
    </item>
    <item>
      <name>reason</name>
      <nullable>0</nullable>
      <type>mediumtext</type>
      <defaultValue/>
      <label>reason</label>
    </item>
  </unsubscribe>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=supporter_invite') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="50449">
  <supporter_invite>
    <item>
      <name>supporter_invite_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter invite KEY</label>
    </item>
    <item>
      <name>supporter_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter KEY</label>
    </item>
    <item>
      <name>tell_a_friend_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>tell a friend KEY</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <name>Date_Created</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Date Created</label>
    </item>
    <item>
      <name>Date_Accepted</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Date Accepted</label>
    </item>
    <item>
      <values>Pending,Accepted,Declined,Maybe,Already a supporter</values>
      <name>Status</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Status</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>Email</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Email</label>
    </item>
    <item>
      <maxlength>32</maxlength>
      <name>First_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>First Name</label>
    </item>
    <item>
      <maxlength>64</maxlength>
      <name>Last_Name</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Last Name</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>Subject</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Subject</label>
    </item>
    <item>
      <maxlength>255</maxlength>
      <name>Content</name>
      <nullable>0</nullable>
      <type>varchar</type>
      <defaultValue/>
      <label>Content</label>
    </item>
    <item>
      <name>event_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>event KEY</label>
    </item>
    <item>
      <name>supporter_event_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter event KEY</label>
    </item>
    <item>
      <name>event_fee_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>event fee KEY</label>
    </item>
  </supporter_invite>
</data>
EOF;
    }
    elseif ($script == '/api/describe2.sjs' && $query[0] == 'object=supporter_event') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <supporter_event>
    <item>
      <name>supporter_event_KEY</name>
      <nullable>1</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter event KEY</label>
    </item>
    <item>
      <name>supporter_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>supporter KEY</label>
    </item>
    <item>
      <name>event_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>event KEY</label>
    </item>
    <item>
      <name>donation_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>donation KEY</label>
    </item>
    <item>
      <name>event_fee_KEY</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue/>
      <label>event fee KEY</label>
    </item>
    <item>
      <name>Last_Modified</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>CURRENT_TIMESTAMP</defaultValue>
      <label>Last Modified</label>
    </item>
    <item>
      <name>Date_Created</name>
      <nullable>1</nullable>
      <type>timestamp</type>
      <defaultValue>Invalid Date</defaultValue>
      <label>Date Created</label>
    </item>
    <item>
      <values>Supporter,Organizer,Volunteer</values>
      <name>_Type</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Type</label>
    </item>
    <item>
      <values>Invited,Waiting List,Viewed Invite,Signed Up,Not Attending,Unsure,Attended,Cancelled,Awaiting Payment,Invoiced,Confirmed</values>
      <name>_Status</name>
      <nullable>0</nullable>
      <type>enum</type>
      <defaultValue/>
      <label>Status</label>
    </item>
    <item>
      <name>Additional_Attendees</name>
      <nullable>0</nullable>
      <type>int</type>
      <defaultValue>0</defaultValue>
      <label>Additional Attendees</label>
    </item>
    <item>
      <name>Feedback</name>
      <nullable>0</nullable>
      <type>mediumtext</type>
      <defaultValue/>
      <label>Feedback</label>
    </item>
  </supporter_event>
</data>
EOF;
    }
    elseif (($query[0] == 'object=event') && isset($query[1]) && $query[1] == 'condition=event_KEY=1') {
      $limit = variable_get('salsa_entity_mock_max_attendee', 2);
      $paid = variable_get('salsa_entity_mock_event_payment', 'true');
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <event>
    <item>
      <event_KEY>1</event_KEY>
      <organization_KEY>50449</organization_KEY>
      <chapter_KEY/>
      <national_event_KEY/>
      <distributed_event_KEY/>
      <supporter_KEY/>
      <Reference_Name>paid_waitinglist_FBConnect_Guests</Reference_Name>
      <Event_Name>Test Event</Event_Name>
      <Last_Modified>Fri Apr 26 2013 14:15:35 GMT-0000 (UTC)</Last_Modified>
      <Date_Created>Mon Mar 04 2013 16:34:52 GMT-0000 (UTC)</Date_Created>
      <Description>&lt;p&gt;&amp;#160;&lt;/p&gt;</Description>
      <Address>Kingsroad. 70</Address>
      <City>London</City>
      <State/>
      <Zip>10058</Zip>
      <Country>UK</Country>
      <Directions/>
      <Header/>
      <Footer/>
      <PRIVATE_Zip_Plus_4/>
      <Start>Fri Mar 29 2013 13:00:00 GMT-0000 (UTC)</Start>
      <End>Sun Mar 31 2013 13:00:00 GMT-0000 (UTC)</End>
      <Deadline/>
      <Recurrence_Frequency/>
      <Recurrence_Interval/>
      <Contact_Email/>
      <No_Registration_BOOLVALUE>false</No_Registration_BOOLVALUE>
      <No_Registration>false</No_Registration>
      <Guests_allowed_BOOLVALUE>false</Guests_allowed_BOOLVALUE>
      <Guests_allowed>false</Guests_allowed>
      <Maximum_Attendees>$limit</Maximum_Attendees>
      <Maximum_Waiting_List_Size>2</Maximum_Waiting_List_Size>
      <Hide_Standard_Map_BOOLVALUE>false</Hide_Standard_Map_BOOLVALUE>
      <Hide_Standard_Map>false</Hide_Standard_Map>
      <Map_URL/>
      <Status>Active</Status>
      <This_Event_Costs_Money_BOOLVALUE>$paid</This_Event_Costs_Money_BOOLVALUE>
      <This_Event_Costs_Money>$paid</This_Event_Costs_Money>
      <Ticket_Price/>
      <merchant_account_KEY/>
      <Default_Tracking_Code>TEST</Default_Tracking_Code>
      <redirect_path/>
      <Request>__v2__,anrede,First_Name,Last_Name,Email,Organization,Street,Street_2,City,State,Country,Zip</Request>
      <Required>First_Name,Last_Name,Email</Required>
      <groups_KEYS>0,109392</groups_KEYS>
      <Automatically_add_to_Groups_BOOLVALUE>false</Automatically_add_to_Groups_BOOLVALUE>
      <Automatically_add_to_Groups>false</Automatically_add_to_Groups>
      <Display_to_Chapters_BOOLVALUE>false</Display_to_Chapters_BOOLVALUE>
      <Display_to_Chapters>false</Display_to_Chapters>
      <Request_Additional_Attendees_BOOLVALUE>false</Request_Additional_Attendees_BOOLVALUE>
      <Request_Additional_Attendees>false</Request_Additional_Attendees>
      <One_Column_Layout_BOOLVALUE>false</One_Column_Layout_BOOLVALUE>
      <One_Column_Layout>false</One_Column_Layout>
      <Reminder_Status>Reminders have been sent</Reminder_Status>
      <Reminder_Hours>24</Reminder_Hours>
      <Latitude>47.3945883000</Latitude>
      <Longitude>8.4745753000</Longitude>
      <address_md5sum>92ffdbc862dd6f9b03e6f01a42ccb256</address_md5sum>
      <Template/>
      <Allow_Guest_Signup_BOOLVALUE>true</Allow_Guest_Signup_BOOLVALUE>
      <Allow_Guest_Signup>true</Allow_Guest_Signup>
      <Maximum_Donation_Amount>1000.00</Maximum_Donation_Amount>
      <Confirmation_Text>THIS IS REQUIRED</Confirmation_Text>
      <supporter_picture_KEY/>
      <supporter_my_donate_page_KEY/>
      <Location_Common_Name>Vauxhall</Location_Common_Name>
      <Facebook_Connect_BOOLVALUE>true</Facebook_Connect_BOOLVALUE>
      <Facebook_Connect>true</Facebook_Connect>
      <zoom_level>14</zoom_level>
      <Invoice_Option_BOOLVALUE>true</Invoice_Option_BOOLVALUE>
      <Invoice_Option>true</Invoice_Option>
      <Facebook_ID/>
      <linkRequest/>
      <linkRequired/>
      <Publish_To_Commons_BOOLVALUE>false</Publish_To_Commons_BOOLVALUE>
      <Publish_To_Commons>false</Publish_To_Commons>
      <key>1</key>
      <object>event</object>
    </item>
    <count>1</count>
  </event>
</data>
EOF;
    }
    elseif ($query[0] == 'object=supporter_event' && isset($query[1]) && $query[1] == 'condition=supporter_KEY=999'
      && isset($query[2]) && $query[2] == 'condition=event_KEY=1') {
      $response = <<<EOF
<?xml version="1.0"?>
<data organization_KEY="11111">
  <supporter_event>
    <item>
      <supporter_event_KEY>1</supporter_event_KEY>
      <organization_KEY>11111</organization_KEY>
      <chapter_KEY/>
      <supporter_KEY>1</supporter_KEY>
      <event_KEY>1</event_KEY>
      <donation_KEY/>
      <event_fee_KEY/>
      <Last_Modified>Tue Mar 05 2013 10:50:15 GMT-0000 (UTC)</Last_Modified>
      <Date_Created>Tue Mar 05 2013 10:50:15 GMT-0000 (UTC)</Date_Created>
      <_Type>Supporter</_Type>
      <_Status>Signed Up</_Status>
      <Additional_Attendees>0</Additional_Attendees>
      <Feedback/>
      <key>1</key>
      <object>supporter_event</object>
    </item>
    <count>1</count>
  </supporter_event>
</data>
EOF;
    }
    elseif ($query[0] == 'json' && isset($query[1]) && $query[1] = 'object=event'
      && isset($query[2]) && $query[2] == 'condition=event_KEY=1'
      && isset($query[3]) && $query[3] == 'include=' . urlencode('required$groups_KEYS')) {
      $response = '[{"required$groups_KEYS": "0,1"}]';
    }
    elseif ($script == '/save') {
      // Store data.
      $saved = variable_get('salsa_entity_mock_saved', array());
      $saved_history = variable_get('salsa_entity_mock_saved_history', array());
      // Always use a new key, start of with 999.
      $data = array();
      $object = NULL;
      foreach ($query as $param) {
        if (strpos($param, '=') !== FALSE) {
          list($name, $value) = explode('=', $param, 2);
          $data[$name] = urldecode($value);
          if ($name == 'object') {
            $object = $value;
          }
        }
      }

      $key = empty($saved[$object]) ? 999 : max(array_keys($saved[$object])) + 1;
      if (empty($data[$object . '_KEY'])) {
        $data[$object . '_KEY'] = $key;
        $data['key'] = $key;
        $data['object'] = $object;
      }
      else {
        $key = $data[$object . '_KEY'];
        $data['key'] = $key;
      }

      // Add Last Modified timestamp.
      $data['Last_Modified'] = date('r');

      $saved_history[] = $data;
      $saved[$object][$key] = $data;
      variable_set('salsa_entity_mock_saved', $saved);
      variable_set('salsa_entity_mock_saved_history', $saved_history);
      return '<data>
            <success object="supporter" key="' . $key . '">Modified entry ' . $key . '</success>
            </data>';
    }
    elseif ($script == '/api/getObjects.sjs' && isset($query[1]) && preg_match('/_KEY=([0-9]{3,4})/', $query[1], $matches)) {
      $saved = variable_get('salsa_entity_mock_saved', array());
      list(, $object) = explode('=', $query[0], 2);
      $key = $matches[1];

      if (isset($saved[$object][$key])) {
      $response = <<<EOF
<?xml version="1.0"?>
  <data organization_KEY="11111">
    <$object>
      <item>
        <chapter_KEY/>
EOF;

        foreach ($saved[$object][$key] as $name => $value) {
          $response .= "<$name>" . htmlentities($value) . "</$name>\n";
        }

        $response .= <<<EOF
      </item>
    <count>1</count>
  </$object>
</data>
EOF;
      }
    }
    // IN support.
    elseif ($script == '/api/getObjects.sjs' && isset($query[1]) && preg_match('/_KEYIN(.+)/', $query[1], $matches)) {
      $saved = variable_get('salsa_entity_mock_saved', array());
      list(, $name) = explode('=', $query[0], 2);
      $keys = array_map('trim', explode(',',urldecode($matches[1])));
      if (isset($saved[$name])) {
        $objects = array_intersect_key($saved[$name], array_flip($keys));

        if ($objects) {
          $count = count($objects);
          $response = <<<EOF
<?xml version="1.0"?>
  <data organization_KEY="11111">
    <$name>
EOF;
          foreach ($objects as $key => $object) {
            $response .= '
      <item>
        <chapter_KEY/>
';

            foreach ($object as $property => $value) {
              $response .= "<$property>" . htmlentities($value) . "</$property>\n";
            }

            $response .= '
      </item>
';
          }

          $response = <<<EOF
    <count>$count</count>
  </$name>
</data>
EOF;
        }
      }
    }

    return $response;
  }

  public function deleteTag($object, $key, $tag) {
    // @todo: Implement this.
  }

  public function upload($file, $properties) {
    // @todo: Implement this.
  }

  /**
   * Implements SalsaAPIInterface::getCount().
   */
  public function getCount($object, array $conditions = array(), $columnCount = NULL) {
    $mock_counts = variable_get('salsa_entity_mock_counts', array());
    $condition_parts = array();
    foreach ($conditions as $name => $condition) {
      $condition_parts[] = $name . $condition['#operator'] . $condition['#value'];
    }
    $condition_string = implode('&', $condition_parts);
    if (isset($mock_counts[$object][$condition_string])) {
      return $mock_counts[$object][$condition_string];
    }
    return 0;
  }
}
