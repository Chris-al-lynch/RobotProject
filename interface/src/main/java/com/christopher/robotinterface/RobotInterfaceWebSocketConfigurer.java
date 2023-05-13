
package com.christopher.robotinterface;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.socket.config.annotation.WebSocketConfigurer;
import org.springframework.web.socket.config.annotation.WebSocketHandlerRegistry;
import org.springframework.web.socket.server.standard.ServletServerContainerFactoryBean;

@Configuration
class RobotInterfaceWebSocketConfigurer implements WebSocketConfigurer
{
    @Override
    public void registerWebSocketHandlers( WebSocketHandlerRegistry registry )
    {
        registry.addHandler( new RobotInterfaceBinaryHandler(), "/message" );
    }

    @Bean
    public ServletServerContainerFactoryBean createWebSocketContainer()
    {
        ServletServerContainerFactoryBean container = new ServletServerContainerFactoryBean();
        container.setMaxBinaryMessageBufferSize( 1024000 );
        return container;
    }
}