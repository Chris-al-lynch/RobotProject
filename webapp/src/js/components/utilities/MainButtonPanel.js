import Col from 'react-bootstrap/Col';
import Container from 'react-bootstrap/Container';

import ConfigButton from "./ConfigButton";
import PlayButton from "./PlayButton";

/**
 * A panel for holding the buttons on the main page.
 */
export default function MainButtonPanel()
{
   return( <Container>
             <Col>
               <ConfigButton />
               <PlayButton />
             </Col>
           </Container>
         );
}