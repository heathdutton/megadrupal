<CustomerAddRq>
  <CustomerAdd>
    <Name><?php print $billing['last_name']  . ', ' . $billing['first_name'] ?></Name>        
    <FirstName><?php print $billing['first_name'] ?></FirstName>
    <LastName><?php print $billing['last_name'] ?></LastName>
    <BillAddress>
      <Addr1><?php print $billing['thoroughfare'] ?></Addr1>
      <Addr2><?php print $billing['premise'] ?></Addr2>
      <Addr3><?php print $billing['sub_premise'] ?></Addr3>
      <City><?php print $billing['locality'] ?></City>
      <State><?php print $billing['administrative_area'] ?></State>
      <PostalCode><?php print $billing['postal_code'] ?></PostalCode>
      <Country><?php print $billing['country'] ?></Country>
    </BillAddress>    
    <?php if ($has_shipping): ?>
      <ShipAddress>      
        <Addr1><?php print $shipping['thoroughfare'] ?></Addr1>
        <Addr2><?php print $shipping['premise'] ?></Addr2>
        <Addr3><?php print $shipping['sub_premise'] ?></Addr3>
        <City><?php print $shipping['locality'] ?></City>
        <State><?php print $shipping['administrative_area'] ?></State>
        <PostalCode><?php print $shipping['postal_code'] ?></PostalCode>
        <Country><?php print $shipping['country'] ?></Country>      
      </ShipAddress>
    <?php endif; ?>    
    <Phone><?php print $data['phone'] ?></Phone>
    <Email><?php print $data['email'] ?></Email>
    <Contact><?php print $billing['name_line'] ?></Contact>
  </CustomerAdd>
</CustomerAddRq>
