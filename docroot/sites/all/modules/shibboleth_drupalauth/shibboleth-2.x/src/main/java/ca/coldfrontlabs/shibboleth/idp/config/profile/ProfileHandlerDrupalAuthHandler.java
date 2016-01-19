package ca.coldfrontlabs.shibboleth.idp.config.profile;
 
import edu.internet2.middleware.shibboleth.common.config.BaseSpringNamespaceHandler;
 
import ca.coldfrontlabs.shibboleth.idp.config.profile.authn.DrupalAuthHandlerBeanDefinitionParser;
import ca.coldfrontlabs.shibboleth.idp.config.profile.authn.DrupalAuthPreviousSessionHandlerBeanDefinitionParser;
import ca.coldfrontlabs.shibboleth.idp.config.profile.authn.DrupalAuthUsernamePasswordLoginHandlerBeanDefinitionParser;
 
public class ProfileHandlerDrupalAuthHandler extends BaseSpringNamespaceHandler {
 
     /** Namespace URI. */
    public static final String NAMESPACE = "http://coldfrontlabs.ca/shibboleth/authn";
 
    public void init(){
        registerBeanDefinitionParser(DrupalAuthHandlerBeanDefinitionParser.SCHEMA_TYPE,
                new DrupalAuthHandlerBeanDefinitionParser());

        registerBeanDefinitionParser(DrupalAuthUsernamePasswordLoginHandlerBeanDefinitionParser.SCHEMA_TYPE,
                new DrupalAuthUsernamePasswordLoginHandlerBeanDefinitionParser());
    }
}
