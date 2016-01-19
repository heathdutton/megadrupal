package ca.coldfrontlabs.shibboleth.idp.config.profile.authn;

import javax.xml.namespace.QName;

import org.opensaml.xml.util.DatatypeHelper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.support.BeanDefinitionBuilder;
import org.w3c.dom.Element;

import ca.coldfrontlabs.shibboleth.idp.config.profile.ProfileHandlerDrupalAuthHandler;
import edu.internet2.middleware.shibboleth.idp.config.profile.authn.AbstractLoginHandlerBeanDefinitionParser;

/**
 * Spring bean definition parser for username/password authentication handlers.
 */
public class DrupalAuthUsernamePasswordLoginHandlerBeanDefinitionParser extends
        AbstractLoginHandlerBeanDefinitionParser {

    /** Schema type. */
    public static final QName SCHEMA_TYPE = new QName(ProfileHandlerDrupalAuthHandler.NAMESPACE, "DrupalAuthUsernamePassword");
    
    /** Class logger. */
    private final Logger log = LoggerFactory.getLogger(DrupalAuthUsernamePasswordLoginHandlerBeanDefinitionParser.class);

    /** {@inheritDoc} */
    protected Class getBeanClass(Element element) {
        return DrupalAuthUsernamePasswordLoginHandlerFactoryBean.class;
    }

    /** {@inheritDoc} */
    protected void doParse(Element config, BeanDefinitionBuilder builder) {
        super.doParse(config, builder);

        builder.addPropertyValue("authenticationServletURL", DatatypeHelper.safeTrim(config.getAttributeNS(null, "authenticationServletURL")));
        builder.addPropertyValue("authCookieName", DatatypeHelper.safeTrim(config.getAttributeNS(null,"authCookieName")));
        builder.addPropertyValue("authValidationEndpoint", DatatypeHelper.safeTrim(config.getAttributeNS(null,"authValidationEndpoint")));
        builder.addPropertyValue("xforwardedHeader", DatatypeHelper.safeTrim(config.getAttributeNS(null,"xforwardedHeader")));
        builder.addPropertyValue("validateSessionIP", DatatypeHelper.safeTrim(config.getAttributeNS(null,"validateSessionIP")));

        String jaasConfigurationURL = DatatypeHelper.safeTrim(config.getAttributeNS(null, "jaasConfigurationLocation"));
        log.debug("Setting JAAS configuration file to: {}", jaasConfigurationURL);
        System.setProperty("java.security.auth.login.config", jaasConfigurationURL);
    }
}
