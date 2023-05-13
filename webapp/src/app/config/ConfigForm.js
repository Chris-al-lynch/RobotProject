import { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import Button from 'react-bootstrap/Button';
import Form from 'react-bootstrap/Form';
import SubmitConfirmation from '../components/popups/SubmitConfirmation';
import { addNewConfig } from '../backend/backend'

export default function ConfigForm()
{
   const [name, setName] = useState( "" );
   const [interfaceAddress, setInterfaceAddress] = useState( "" );
   const [interfacePort, setInterfacePort] = useState( "" );
   const [robotAddress, setRobotAddress] = useState( "" );
   const [robotPort, setRobotPort] = useState( "" );
   const [description, setDescription] = useState( "" );
   const [showSubmitConfirm, setShowSubmitConfirm] = useState( false );
   const [dataToSubmit, setDataToSubmit] = useState([]);

   const navigate = useNavigate();

   function onSubmit( e )
   {
      e.preventDefault();

      /* TODO: Validate the fields */

      console.log( "onSubmit()" )
      setDataToSubmit( [ {key: 1, label: "Name: ", value: name},
                         {key: 2, label: "Interface Address: ", value: interfaceAddress},
                         {key: 3, label: "Interface Port: ", value: interfacePort},
                         {key: 4, label: "Robot Address: ", value: robotAddress},
                         {key: 5, label: "Robot Port: ", value: robotPort},
                         {key: 6, label: "Description: ", value: description} ] );
      setShowSubmitConfirm( true );
   }

   function onCancel()
   {
      navigate( -1 );
   }

   function handleSubmit( data )
   {
      console.log( "handleSubmit(): " )
      data.map( (data) => console.log( data.label + data.value ) );

      /* TODO: Add spinner for database update. */
      setShowSubmitConfirm( false );

      addNewConfig( data )

      navigate( -1 );
   }

   function handleSubmitConfirmationClose()
   {
      setShowSubmitConfirm( false );
   }

   const onNameInput = ( {target:{value}} ) => setName( value );
   const onInterfaceAddressInput = ( {target:{value}} ) => setInterfaceAddress( value );
   const onInterfacePortInput = ( {target:{value}} ) => setInterfacePort( value );
   const onRobotAddressInput = ( {target:{value}} ) => setRobotAddress( value );
   const onRobotPortInput = ( {target:{value}} ) => setRobotPort( value );
   const onDescriptionInput = ( {target:{value}} ) => setDescription( value );

   return( 
           <div className='text-light'>
             <h1>Configuration</h1>
             <Form onSubmit={onSubmit}>
               <Form.Label>Name of Robot Configuration</Form.Label>
               <Form.Control as='input' type='text'
                             onChange={onNameInput}
                             value={name}
                             placeholder="ex. Fred" />
               <br />
               <Form.Label>Address of Interface</Form.Label>
               <Form.Control as='input' type='text' id="interfaceAddress"
                             onChange={onInterfaceAddressInput}
                             value={interfaceAddress}
                             placeholder="ex. 123.456.78.90 or 1234:abcd:56ef:7890 or my.example.com" />
               <br />
               <Form.Label>Port for Address of Interface</Form.Label>
               <Form.Control as='input' type='text' id="interfacePort"
                             onChange={onInterfacePortInput}
                             value={interfacePort}
                             placeholder="ex. 1234" />
               <br />
               <Form.Label>Address of Robot</Form.Label>
               <Form.Control as='input' type='text' id="robotAddress"
                             onChange={onRobotAddressInput}
                             value={robotAddress}
                             placeholder="ex. 123.456.78.90 or 1234:abcd:56ef:7890 or my.example.com" />
               <br />
               <Form.Label>Port for Address of Robot</Form.Label>
               <Form.Control as='input' type='text' id="robotPort"
                             onChange={onRobotPortInput}
                             value={robotPort}
                             placeholder="ex. 1234" />
               <br />
               <Form.Label>Description</Form.Label>
               <Form.Control as='input' type='text' id="description"
                             onChange={onDescriptionInput}
                             value={description} />
               <br />
               <Button type='submit'>Submit</Button>
               <Button type='cancel' onClick={onCancel}>Cancel</Button>
             </Form>
             <div>
               <SubmitConfirmation dataToSubmit={dataToSubmit}
                                   showPopup={showSubmitConfirm}
                                   handleSubmit={handleSubmit}
                                   handleClose={handleSubmitConfirmationClose}
                />
             </div>
           </div>
          );
}