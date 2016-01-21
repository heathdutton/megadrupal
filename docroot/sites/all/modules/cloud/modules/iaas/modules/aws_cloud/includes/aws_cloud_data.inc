<?php

/**
 * @file
 * AWS cloud data. This module handles all AWS cloud-related
 * data including pricing, and etc.
 *
 */

// created by yas 2015/05/08

/**
 * Implementation of hook_aws_cloud_data()
 * This is aws_cloud's default implementation.
 * This hook returns predefined data about
 * all the sub-clouds defined.
 */
function aws_cloud_aws_cloud_data() {

  $data = array();
  $data['amazon'] = array(
    'name' => AWS_CLOUD_AMAZON_INSTANCE_NAME,
    'instance_types' => array(

      /* Older Generation */
      't1.micro'     => 't1.micro'   ,
      'm1.small'     => 'm1.small'   ,
      'm1.medium'    => 'm1.medium'  ,
      'm1.large'     => 'm1.large'   ,
      'm1.xlarge'    => 'm1.xlarge'  ,
      'c1.medium'    => 'c1.medium'  ,
      'c1.xlarge'    => 'c1.xlarge'  ,
      'm2.xlarge'    => 'm2.xlarge'  ,
      'm2.2xlarge'   => 'm2.2xlarge' ,
      'm2.4xlarge'   => 'm2.4xlarge' ,
      'cc1.4xlarge'  => 'cc1.4xlarge',
      'cc2.8xlarge'  => 'cc2.8xlarge',
      'cg1.4xlarge'  => 'cg1.4xlarge',

      /* Current Generation */
      't2.micro'     => 't2.micro'   ,
      't2.small'     => 't2.small'   ,
      't2.medium'    => 't2.medium'  ,
      'm3.medium'    => 'm3.medium'  ,
      'm3.large'     => 'm3.large'   ,
      'm3.xlarge'    => 'm3.xlarge'  ,
      'm3.2xlarge'   => 'm3.2xlarge' ,
      'c4.large'     => 'c4.large'   ,
      'c4.xlarge'    => 'c4.xlarge'  ,
      'c4.2xlarge'   => 'c4.2xlarge' ,
      'c4.4xlarge'   => 'c4.4xlarge' ,
      'c4.8xlarge'   => 'c4.8xlarge' ,
      'c3.large'     => 'c3.large'   ,
      'c3.xlarge'    => 'c3.xlarge'  ,
      'c3.2xlarge'   => 'c3.2xlarge' ,
      'c3.4xlarge'   => 'c3.4xlarge' ,
      'c3.8xlarge'   => 'c3.8xlarge' ,
      'g2.2xlarge'   => 'g2.2xlarge' ,
      'g2.8xlarge'   => 'g2.8xlarge' ,
      'r3.large'     => 'r3.large'   ,
      'r3.xlarge'    => 'r3.xlarge'  ,
      'r3.2xlarge'   => 'r3.2xlarge' ,
      'r3.4xlarge'   => 'r3.4xlarge' ,
      'r3.8xlarge'   => 'r3.8xlarge' ,
      'i2.xlarge'    => 'i2.xlarge'  ,
      'i2.2xlarge'   => 'i2.2xlarge' ,
      'i2.4xlarge'   => 'i2.4xlarge' ,
      'i2.8xlarge'   => 'i2.8xlarge' ,
      'd2.xlarge'    => 'd2.xlarge'  ,
      'd2.2xlarge'   => 'd2.2xlarge' ,
      'd2.4xlarge'   => 'd2.4xlarge' ,
      'd2.8xlarge'   => 'd2.8xlarge' ,
    ),

    /* Pricing data as of 04/09/2012 */
    /* Only reflected for us-east    */
    'cloud_pricing_data' => array(

      /* Older Generation */
      // Micro On-Demand Instances
      't1.micro'  => array(
        'instance_type'      => 't1.micro'                                    ,
        'description'        => t('Micro')                                    ,
        'linux_or_unix_cost' => '0.02'                                        ,
        'windows_cost'       => '0.03'                                        ,
      ),
      // Standard On-Demand Instances
      'm1.small'  => array(
        'instance_type'      => 'm1.small'                                    ,
        'description'        => t('Small (Default)')                          ,
        'linux_or_unix_cost' => '0.044'                                       ,
        'windows_cost'       => '0.629'                                       ,
      ),
      'm1.medium'  => array(
        'instance_type'      => 'm1.medium'                                   ,
        'description'        => t('Medium')                                   ,
        'linux_or_unix_cost' => '0.087'                                       ,
        'windows_cost'       => '0.744'                                       ,
      ),
      'm1.large'  => array(
        'instance_type'      => 'm1.large'                                    ,
        'description'        => t('Large')                                    ,
        'linux_or_unix_cost' => '0.175'                                       ,
        'windows_cost'       => '0.955'                                       ,
      ),
      'm1.xlarge' => array(
        'instance_type'      => 'm1.xlarge'                                   ,
        'description'        => t('Extra Large')                              ,
        'linux_or_unix_cost' => '0.350'                                       ,
        'windows_cost'       => '1.362'                                       ,
      ),

      // Hi-Memory On-Demand Instances
      'm2.xlarge' => array(
        'instance_type'      => 'm2.xlarge'                                   ,
        'description'        => t('Hi-Memory On-Demand Extra Large')          ,
        'linux_or_unix_cost' => '0.245'                                       ,
        'windows_cost'       => '1.084'                                       ,
      ),
      'm2.2xlarge' => array(
        'instance_type'      => 'm2.2xlarge'                                  ,
        'description'        => t('Hi-Memory On-Demand Double Extra Large')   ,
        'linux_or_unix_cost' => '0.490'                                       ,
        'windows_cost'       => '1.489'                                       ,
      ),
      'm2.4xlarge' => array(
        'instance_type'      => 'm2.4xlarge'                                  ,
        'description'        => t('Hi-Memory On-Demand Quadruple Extra Large'),
        'linux_or_unix_cost' => '0.980'                                       ,
        'windows_cost'       => '2.976'                                       ,
      ),

      // Hi-CPU On-Demand Instances
      'c1.medium' => array(
        'instance_type'      => 'c1.medium'                                   ,
        'description'        => t('High-CPU Medium')                          ,
        'linux_or_unix_cost' => '0.130'                                       ,
        'windows_cost'       => '0'                                           ,
      ),
      'c1.xlarge' => array(
        'instance_type'      => 'c1.xlarge'                                   ,
        'description'        => t('High-CPU Extra Large')                     ,
        'linux_or_unix_cost' => '0.520'                                       ,
        'windows_cost'       => '2.124'                                       ,
      ),
      // Cluster Compute Instances
      'cc1.4xlarge' => array(
        'instance_type'      => 'cc1.4xlarge'                                 ,
        'description'        => t('Cluster Compute Quadruple Extra Large')    ,
        'linux_or_unix_cost' => '1.30'                                        ,
        'windows_cost'       => '1.61'                                        ,
      ),
      'cc2.8xlarge' => array(
        'instance_type'      => 'cc2.8xlarge'                                 ,
        'description'        => t('Cluster Compute Eight Extra Large')        ,
        'linux_or_unix_cost' => '2.000'                                       ,
        'windows_cost'       => '2.97'                                        ,
      ),
      'cg1.4xlarge' => array(
        'instance_type'      => 'cg1.4xlarge'                                 ,
        'description'        => t('Cluster GPU Quadruple Extra Large')        ,
        'linux_or_unix_cost' => '2.100'                                       ,
        'windows_cost'       => '3.850'                                       ,
      ),

      /* Current Generation */
      't2.micro'  => array(
        'instance_type'      => 't2.micro',
        'description'        => t('1 x vCPU, Variant ECU, 1 GB Memory, EBS Only'),
        'linux_or_unix_cost' => '0.013',
        'windows_cost'       => '0.018',
      ),
      't2.small'  => array(
        'instance_type'      => 't2.small',
        'description'        => t('1 x vCPU, Variant ECU, 2 GB Memory, EBS Only'),
        'linux_or_unix_cost' => '0.026',
        'windows_cost'       => '0.036',
      ),
      't2.medium'  => array(
        'instance_type'      => 't2.medium',
        'description'        => t('2 x vCPU, Variant ECU, 4 GB Memory, EBS Only'),
        'linux_or_unix_cost' => '0.052',
        'windows_cost'       => '0.072',
      ),
      'm3.medium'  => array(
        'instance_type'      => 'm3.medium',
        'description'        => t('1 x vCPU, 3 x ECU, 3.75 GB Memory, 1 x 4 GB SSD'),
        'linux_or_unix_cost' => '0.070',
        'windows_cost'       => '0.133',
      ),
      'm3.large'  => array(
        'instance_type'      => 'm3.large',
        'description'        => t('2 x vCPU, 6.5 x ECU, 7.5 GB Memory, 1 x 32 GB SSD'),
        'linux_or_unix_cost' => '0.140',
        'windows_cost'       => '0.266',
      ),
      'm3.xlarge'  => array(
        'instance_type'      => 'm3.xlarge',
        'description'        => t('4 x vCPU, 13 x ECU, 15 GB Memory, 2 x 40 GB SSD'),
        'linux_or_unix_cost' => '0.280',
        'windows_cost'       => '0.532',
      ),
      'm3.2xlarge'  => array(
        'instance_type'      => 'm3.2xlarge',
        'description'        => t('8 x vCPU, 26 x ECU, 30 GB Memory, 2 x 80 GB SSD'),
        'linux_or_unix_cost' => '0.560',
        'windows_cost'       => '1.064',
      ),
      // Optimized for Computing
      'c4.large'  => array(
        'instance_type'      => 'c4.large',
        'description'        => t('2 x vCPU, 8 x ECU, 3.75 GB Memory, EBS Only'),
        'linux_or_unix_cost' => '0.116',
        'windows_cost'       => '0.199',
      ),
      'c4.xlarge'  => array(
        'instance_type'      => 'c4.xlarge',
        'description'        => t('4 x vCPU, 16 x ECU, 7.5 GB Memory, EBS Only'),
        'linux_or_unix_cost' => '0.232',
        'windows_cost'       => '0.398',
      ),
      'c4.2xlarge'  => array(
        'instance_type'      => 'c4.2xlarge',
        'description'        => t('8 x vCPU, 31 x ECU, 15 GB Memory, EBS Only'),
        'linux_or_unix_cost' => '0.464',
        'windows_cost'       => '0.796',
      ),
      'c4.4xlarge'  => array(
        'instance_type'      => 'c4.4xlarge',
        'description'        => t('16 x vCPU, 62 x ECU, 30 GB Memory, EBS Only'),
        'linux_or_unix_cost' => '0.928',
        'windows_cost'       => '1.592',
      ),
      'c4.8xlarge'  => array(
        'instance_type'      => 'c4.8xlarge',
        'description'        => t('36 x vCPU, 132 x ECU, 60 GB Memory, EBS Only'),
        'linux_or_unix_cost' => '1.856',
        'windows_cost'       => '3.184',
      ),
      'c3.large'  => array(
        'instance_type'      => 'c3.large',
        'description'        => t('2 x vCPU, 7 x ECU, 3.75 GB Memory, 2 x 16 GB SSD'),
        'linux_or_unix_cost' => '0.105',
        'windows_cost'       => '0.188',
      ),
      'c3.xlarge'  => array(
        'instance_type'      => 'c3.xlarge',
        'description'        => t('4 x vCPU, 14 x ECU, 7.5 GB Memory, 2 x 40 GB SSD'),
        'linux_or_unix_cost' => '0.210',
        'windows_cost'       => '0.376',
      ),
      'c3.2xlarge'  => array(
        'instance_type'      => 'c3.2xlarge',
        'description'        => t('8 x vCPU, 28 x ECU, 15 GB Memory, 2 x 80 GB SSD'),
        'linux_or_unix_cost' => '0.420',
        'windows_cost'       => '0.752',
      ),
      'c3.4xlarge'  => array(
        'instance_type'      => 'c3.4xlarge',
        'description'        => t('16 x vCPU, 55 x ECU, 30 GB Memory, 2 x 160 GB SSD'),
        'linux_or_unix_cost' => '0.840',
        'windows_cost'       => '1.504',
      ),
      'c3.8xlarge'  => array(
        'instance_type'      => 'c3.8xlarge',
        'description'        => t('32 x vCPU, 108 x ECU, 60 GB Memory, 2 x 320 GB SSD'),
        'linux_or_unix_cost' => '1.680',
        'windows_cost'       => '3.008',
      ),
      // GPU Instance
      'g2.2xlarge'  => array(
        'instance_type'      => 'g2.2xlarge',
        'description'        => t('8 x vCPU, 26 x ECU, 15 GB Memory, 60 GB SSD'),
        'linux_or_unix_cost' => '0.650',
        'windows_cost'       => '0.767',
      ),
      'g2.8xlarge'  => array(
        'instance_type'      => 'g2.8xlarge',
        'description'        => t('32 x vCPU, 104 x ECU, 60 GB Memory, 2 x 120 GB SSD'),
        'linux_or_unix_cost' => '2.600',
        'windows_cost'       => '2.878',
      ),
      // Optimized for Memory
      'r3.large'  => array(
        'instance_type'      => 'r3.large',
        'description'        => t('2 x vCPU, 6.5 x ECU, 15 GB Memory, 1 x 32 GB SSD'),
        'linux_or_unix_cost' => '0.175',
        'windows_cost'       => '0.300',
      ),
      'r3.xlarge'  => array(
        'instance_type'      => 'r3.xlarge',
        'description'        => t('4 x vCPU, 13 x ECU, 30.5 GB Memory, 1 x 80 GB SSD'),
        'linux_or_unix_cost' => '0.350',
        'windows_cost'       => '0.600',
      ),
      'r3.2xlarge'  => array(
        'instance_type'      => 'r3.2xlarge',
        'description'        => t('8 x vCPU, 26 x ECU, 61 GB Memory, 1 x 160 GB SSD'),
        'linux_or_unix_cost' => '0.700',
        'windows_cost'       => '1.080',
      ),
      'r3.4xlarge'  => array(
        'instance_type'      => 'r3.4xlarge',
        'description'        => t('16 x vCPU, 52 x ECU, 122 GB Memory, 1 x 320 GB SSD'),
        'linux_or_unix_cost' => '1.400',
        'windows_cost'       => '1.944',
      ),
      'r3.8xlarge'  => array(
        'instance_type'      => 'r3.8xlarge',
        'description'        => t('32 x vCPU, 104 x ECU, 244 GB Memory, 2 x 320 GB SSD'),
        'linux_or_unix_cost' => '2.800',
        'windows_cost'       => '3.500',
      ),
      // Optimized for Storage
      'i2.xlarge'  => array(
        'instance_type'      => 'i2.xlarge',
        'description'        => t('4 x vCPU, 14 x ECU, 30.5 GB Memory, 1 x 800 GB SSD'),
        'linux_or_unix_cost' => '0.853',
        'windows_cost'       => '0.973',
      ),
      'i2.2xlarge'  => array(
        'instance_type'      => 'i2.2xlarge',
        'description'        => t('8 x vCPU, 27 x ECU, 61 GB Memory, 2 x 800 GB SSD'),
        'linux_or_unix_cost' => '1.705',
        'windows_cost'       => '1.946',
      ),
      'i2.4xlarge'  => array(
        'instance_type'      => 'i2.4xlarge',
        'description'        => t('16 x vCPU, 53 x ECU, 122 GB Memory, 4 x 800 GB SSD'),
        'linux_or_unix_cost' => '3.410',
        'windows_cost'       => '3.891',
      ),
      'i2.8xlarge'  => array(
        'instance_type'      => 'i2.8xlarge',
        'description'        => t('32 x vCPU, 104 x ECU, 244 GB Memory, 8 x 800 GB SSD'),
        'linux_or_unix_cost' => '6.820',
        'windows_cost'       => '7.782',
      ),
      'd2.xlarge'  => array(
        'instance_type'      => 'd2.xlarge',
        'description'        => t('4 x vCPU, 14 x ECU, 30.5 GB Memory, 3 x 2,000 GB HDD'),
        'linux_or_unix_cost' => '0.690',
        'windows_cost'       => '0.821',
      ),
      'd2.2xlarge'  => array(
        'instance_type'      => 'd2.2xlarge',
        'description'        => t('8 x vCPU, 28 x ECU, 61 GB Memory, 6 x 2,000 GB HDD'),
        'linux_or_unix_cost' => '1.380',
        'windows_cost'       => '1.601',
      ),
      'd2.4xlarge'  => array(
        'instance_type'      => 'd2.4xlarge',
        'description'        => t('16 x vCPU, 56 x ECU, 122 GB Memory, 12 x 2,000 GB HDD'),
        'linux_or_unix_cost' => '2.760',
        'windows_cost'       => '3.062',
      ),
      'd2.8xlarge'  => array(
        'instance_type'      => 'd2.8xlarge',
        'description'        => t('36 x vCPU, 116 x EU, 244 GB Memory, 24 x 2,000 GB HDD'),
        'linux_or_unix_cost' => '5.520',
        'windows_cost'       => '6.198',
      ),
    ),
  );

  /* OpenStack is used for private cloud; so the following pricing info is
   * just imitaging AWS us-east pricing model.
   */
  $data['openstack_nova'] = array(
    'name' => AWS_CLOUD_OPENSTACK_INSTANCE_NAME,
    'instance_types' => array(
      'm1.tiny'   => 'm1.tiny'                      ,
      'm1.small'  => 'm1.small'                     ,
      'm1.medium' => 'm1.medium'                    ,
      'm1.large'  => 'm1.large'                     ,
    ),
    'cloud_pricing_data' => array(
      'm1.tiny' => array(
        'instance_type'      => 'm1.tiny'           ,
        'description'        => t('Tiny')           ,
        'linux_or_unix_cost' => '0.013'             ,
        'windows_cost'       => '0.018'             ,
      ),
      'm1.small' => array(
        'instance_type'      => 'm1.small'          ,
        'description'        => t('Small (Default)'),
        'linux_or_unix_cost' => '0.026'             ,
        'windows_cost'       => '0.036'             ,
      ),
      'm1.medium' => array(
        'instance_type'      => 'm1.medium'         ,
        'description'        => t('Medium')         ,
        'linux_or_unix_cost' => '0.052'             ,
        'windows_cost'       => '0.072'              ,
      ),
      'm1.large' => array(
        'instance_type'      => 'm1.large'          ,
        'description'        => t('Large')          ,
        'linux_or_unix_cost' => '0.140'             ,
        'windows_cost'       => '0.266'             ,
      ),
    ),
  );

  // Eucalyptus
  $data['eucalyptus'] = array(
    'name' => AWS_CLOUD_EUCALYPTUS_INSTANCE_NAME,
    'instance_types' => array(
      'm1.small'  => 'm1.small'                     ,
      'c1.medium' => 'c1.medium'                    ,
      'm1.large'  => 'm1.large'                     ,
      'm1.xlarge' => 'm1.xlarge'                    ,
    ),
    'cloud_pricing_data' => array(
      'm1.small'              => array(
        'instance_type'      => 'm1.small'          ,
        'description'        => t('Small (Default)'),
        'linux_or_unix_cost' => '0.026'             ,
        'windows_cost'       => '0.036'             ,
      ),
      'c1.medium' => array(
        'instance_type'      => 'c1.medium'         ,
        'description'        => t('Medium')         ,
        'linux_or_unix_cost' => '0.052'             ,
        'windows_cost'       => '0.072'             ,
      ),
      'm1.large' => array(
        'instance_type'      => 'm1.large'          ,
        'description'        => t('Large')          ,
        'linux_or_unix_cost' => '0.140'             ,
        'windows_cost'       => '0.072'             ,
      ),
      'm1.xlarge' => array(
        'instance_type'      => 'm1.xlarge'         ,
        'description'        => t('Extra Large')    ,
        'linux_or_unix_cost' => '0.280'             ,
        'windows_cost'       => '0.532'             ,
      ),
    ),
  );

  /* Pricing data as of 2015/05/06 */
  $data['cloudn'] = array(
    'name' => AWS_CLOUD_CLOUDN_INSTANCE_NAME,
    'instance_types' => array(
      't1.micro'  => 't1.micro'                                ,
      'm1.small'  => 'm1.small'                                ,
      'm1.medium' => 'm1.medium'                               ,
      'm1.large'  => 'm1.large'                                ,
      'm1.xlarge' => 'm1.xlarge'                               ,
    ),
    'cloud_pricing_data' => array(
      't1.micro' => array(
        'instance_type'      => 't1.micro'                     ,
        'description'        => t('Plan vQ (1CPU / 0.5GB RAM)'),
        'linux_or_unix_cost' => '0.009'                        ,
        'windows_cost'       => '0.009'                        ,
      ),
      'm1.small' => array(
        'instance_type'      => 'm1.small'                     ,
        'description'        => t('Plan v1 (1CPU / 2GB RAM)')  ,
        'linux_or_unix_cost' => '0.0646'                       ,
        'windows_cost'       => '0.0646'                       ,
      ),
      'm1.medium' => array(
        'instance_type'      => 'm1.medium'                    ,
        'description'        => t('Plan v2 (2CPU / 4GB RAM)')  ,
        'linux_or_unix_cost' => '0.1178'                       ,
        'windows_cost'       => '0.1178'                       ,
      ),
      'm1.large' => array(
        'instance_type'      => 'm1.large'                     ,
        'description'        => t('Plan v4 (4CPU / 8GB RAM)')  ,
        'linux_or_unix_cost' => '0.2356'                       ,
        'windows_cost'       => '0.2356'                       ,
      ),
      'm1.xlarge' => array(
        'instance_type'      => 'm1.xlarge'                    ,
        'description'        => t('Plan v8 (8CPU / 16GB RAM)') ,
        'linux_or_unix_cost' => '0.4712'                       ,
        'windows_cost'       => '0.4712'                       ,
      ),
    ),
  );

  return $data;
}