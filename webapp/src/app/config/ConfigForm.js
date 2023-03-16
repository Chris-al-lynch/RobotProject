import { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import Button from 'react-bootstrap/Button';
import Form from 'react-bootstrap/Form';
import SubmitConfirmation from '../components/popups/SubmitConfirmation';
//import { AddConfig } from '../../database/Database';

export default function ConfigForm()
{
   const [name, setName] = useState( "" );
   const [address, setAddress] = useState( "" );
   const [port, setPort] = useState( "" );
   const [description, setDescription] = useState( "" );
   const [showSubmitConfirm, setShowSubmitConfirm] = useState( false );
   const [dataToSubmit, setDataToSubmit] = useState([]);

   const navigate = useNavigate();

   function onSubmit( e )
   {
      e.preventDefault();

      /* TODO: Validate the fields */

      setDataToSubmit( [ {key: 1, label: "name: ", value: name},
                         {key: 2, label: "address: ", value: address},
                         {key: 3, label: "port: ", value: port},
                         {key: 4, label: "description: ", value: description} ] );
      setShowSubmitConfirm( true );
   }

   function onCancel()
   {
      navigate( -1 );
   }

   function handleSubmit( data )
   {
      console.log( "handleSubmit(): ")
      data.map( (data) => console.log( data.label + data.value ) );

      //AddConfig( data );

      /* TODO: Add spinner for database update. */
      setShowSubmitConfirm( false );

      navigate( -1 );
   }

   function handleSubmitConfirmationClose()
   {
      setShowSubmitConfirm( false );
   }

   const onNameInput = ( {target:{value}} ) => setName( value );
   const onAddressInput = ( {target:{value}} ) => setAddress( value );
   const onPortInput = ( {target:{value}} ) => setPort( value );
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
               <Form.Label>Address of Robot</Form.Label>
               <Form.Control as='input' type='text' id="address"
                             onChange={onAddressInput}
                             value={address}
                             placeholder="ex. 123.456.78.90 or 1234:abcd:56ef:7890 or my.example.com" />
               <br />
               <Form.Label>Port for Address of Robot</Form.Label>
               <Form.Control as='input' type='text' id="port"
                             onChange={onPortInput}
                             value={port}
                             placeholder="ex. 1234" />
               <br />
               <Form.Label>Description</Form.Label>
               <Form.Control as='input' type='text' id="description"
                             onChange={onDescriptionInput}
                             value={description} />
               <br />
               <Button type='submit'>Submit</Button>
               <Button type='cancel' onClick={onCancel}>Submit</Button>
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