package com.christopher.robotinterface;

import org.springframework.web.socket.BinaryMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.BinaryWebSocketHandler;

public class RobotInterfaceBinaryHandler extends BinaryWebSocketHandler
{
    @Override
    public void handleBinaryMessage( WebSocketSession session, BinaryMessage message )
    {
        System.out.println( "Message received." );
    }
}
