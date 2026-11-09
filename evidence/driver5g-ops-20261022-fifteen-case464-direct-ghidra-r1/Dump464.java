import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
public class Dump464 extends GhidraScript {
 public void run() throws Exception {
  for(String raw:getScriptArgs()) {
   Address address=toAddr(raw);
   disassemble(address);
   println("FOCUSED_ADDRESS "+raw);
   for(int i=0;i<8;i++) {
    Instruction instruction=currentProgram.getListing().getInstructionAt(address);
    if(instruction==null){println("NO_DECODE "+address);break;}
    println(address+": "+instruction.toString());
    address=instruction.getMaxAddress().add(1);
   }
  }
 }
}
