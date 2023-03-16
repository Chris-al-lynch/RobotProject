import Button from 'react-bootstrap/Button';
import Modal from 'react-bootstrap/Modal';

export default function DeleteConfirmation( {item, showPopup, handleDelete, handleClose} )
{
    function onDelete()
    {
        handleDelete( item );
    }

    function onCancel()
    {
        handleClose();
    }

    return(

            <div className='DeleteConfirmation' style={ {display: 'block', position: 'initial'} }>
                <Modal show={showPopup} onHide={onCancel} animation={true}>
                  <Modal.Dialog>
                    <Modal.Header closeButton>
                      <Modal.Title>Are you sure you want to delete {item}?</Modal.Title>
                    </Modal.Header>
                  </Modal.Dialog>

                  <Modal.Body>
                    <p>It will be gone forever.</p>
                  </Modal.Body>

                  <Modal.Footer>
                    <Button variant="secondary" onClick={onDelete}>Delete</Button>
                    <Button variant="primary" onClick={onCancel}>Cancel</Button>
                  </Modal.Footer>
                </Modal>
            </div>
          );
}