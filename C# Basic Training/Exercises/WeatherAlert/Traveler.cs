namespace ilrd
{
    public abstract class Traveler
    {
        protected int Threshold { get; set; }

        public bool InPark { get; set; }
        public void React(WeatherEvent weatherEvent_)
        {
            int intensity = weatherEvent_.Intensity;
            if (intensity > Threshold)
            {
                Console.WriteLine($"{ReactionForLeaving()} {intensity}!!");

                //now we leave the park
                InPark = false;
            }
            else
            {
                Console.WriteLine($"{ ReactionForStaying()} {intensity} ...");
            }
        }
        public abstract string ReactionForStaying();
        public abstract string ReactionForLeaving();

    }

    public class Family : Traveler
    {
        public Family() => Threshold = 5;


        public override string ReactionForLeaving()
            => "We're leaving so the kids wont get sick. the storm intensity is level - ";

        public override string ReactionForStaying()
            => "The kids LOVE jumping in the puddles! We won't leave for storm level - ";
    }

    public class YoungCouple : Traveler
    {
        public YoungCouple() => Threshold = 8;

        public override string ReactionForLeaving()
            => "We gotta go, this is like a hurricane of level - ";

        public override string ReactionForStaying()
            => "What a romantic walk in the rain! We're not afraid of storm level - ";
    }

    public class OlderGroup : Traveler
    {
        public OlderGroup() => Threshold = 2;

        public override string ReactionForLeaving()
            => "us old folk need to leave NOW!, the storm reached level - ";

        public override string ReactionForStaying()
            => "Even us old folks can deal with storm intensity level - ";


    }
}