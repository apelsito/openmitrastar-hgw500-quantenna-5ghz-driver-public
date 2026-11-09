import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
public class DumpPick extends GhidraScript {
 public void run() throws Exception {
  Address address=toAddr("0x14dd2");
  Address finish=toAddr("0x14ee6");
  disassemble(address);
  while(address.compareTo(finish)<0){
   Instruction instruction=currentProgram.getListing().getInstructionAt(address);
   if(instruction==null){println("NO_DECODE "+address);break;}
   println(address+": "+instruction.toString());
   address=instruction.getMaxAddress().add(1);
  }
 }
}
