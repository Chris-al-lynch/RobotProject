import Button from 'react-bootstrap/Button';
import Modal from 'react-bootstrap/Modal';

export default function SubmitConfirmation( {dataToSubmit, showPopup, handleSubmit,
                                             handleClose} )
{
    function onSubmit()
    {
        handleSubmit( dataToSubmit );
    }

    function onCancel()
    {
        handleClose();
    }

    return(

            <div className='SubmitConfirmation' style={ {display: 'block', position: 'initial'} }>
                <Modal show={showPopup} onHide={onCancel} animation={true}>
                  <Modal.Dialog>
                    <Modal.Header closeButton>
                      <Modal.Title>Are you sure you want to Submit the follwing?</Modal.Title>
                    </Modal.Header>
                  </Modal.Dialog>

                  <Modal.Body>
                    {dataToSubmit.map( item => <li key={item.key}>{item.label + item.value} </li> ) }
                  </Modal.Body>

                  <Modal.Footer>
                    <Button variant="secondary" onClick={onSubmit}>Submit</Button>
                    <Button variant="primary" onClick={onCancel}>Cancel</Button>
                  </Modal.Footer>
                </Modal>
            </div>
          );
}